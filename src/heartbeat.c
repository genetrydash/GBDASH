#include <gb/gb.h>
#include <gb/hardware.h>
#include <stdio.h>
#include "heartbeat.h"

// Function pointers for callbacks
void (*dobeat)(void) = 0;
void (*dotick)(void) = 0;

// Timer configuration
unsigned char tickinterval = 0;
int tickrate = 0;
unsigned char tac_setting = 0;

unsigned int beatinterval = 0;

// Available Game Boy timer base frequencies
const unsigned int timer_freqs[] = {4096, 16384, 65535};
const unsigned char tac_values[] = {0x04, 0x05, 0x06, 0x07};

// Timer interrupt handler
int currenttick = 0;
void irq_handler(void) {
    currenttick++;
    if (dotick != 0) {
        dotick();
    }

    if (dobeat != 0 && currenttick % beatinterval == 0) {
        dobeat();
        currenttick = 0;
    }
    
}

void initirq(void) {
    add_TIM(irq_handler);
    IE_REG |= TIM_IFLAG;
}

void closeirq(void) {
    remove_TIM(irq_handler);
    IE_REG &= ~TIM_IFLAG;
}

// Set the beat callback
void setbeatcallback(void (*callback)(void)) {
    disable_interrupts();
    dobeat = callback;
    enable_interrupts();
}

// Set the tick callback
void settickcallback(void (*callback)(void)) {
    disable_interrupts();
    dotick = callback;
    enable_interrupts();
}

void setbpm(int bpm) {
    beatinterval = (tickrate * 60) / bpm;
}


unsigned int mul16_8(unsigned int value, unsigned char multiplier) {
    unsigned int result = 0;

    while (multiplier) {
        if (multiplier & 1) {
            result += value;
        }
        value <<= 1;
        multiplier >>= 1;
    }

    return result;
}



unsigned int div16_8(unsigned int dividend, unsigned char divisor) {
    unsigned int quotient = 0;
    unsigned int bit = 1;
    unsigned int denom = divisor;

    // Shift left until denom just <= dividend
    while ((denom << 1) <= dividend && (denom << 1) > denom) {
        denom <<= 1;
        bit <<= 1;
    }

    while (bit != 0) {
        if (dividend >= denom) {
            dividend -= denom;
            quotient |= bit;
        }
        denom >>= 1;
        bit >>= 1;
    }

    return quotient;
}




// Set the tick rate as close as possible to `rate`
void settickrate(unsigned int rate) {
    unsigned char best_interval = 1;
    unsigned char best_tac = 0x04;
    unsigned int best_error = 0xFFFF;
    unsigned char i;

    // Limit interval range to 8..128
    for (i = 0; i < 4; i++) {
        unsigned int base_freq = timer_freqs[i];
        unsigned char interval;
        for (interval = 8; interval <= 200; interval++) {
            unsigned int est_target = mul16_8(rate,interval);
            unsigned int error = (base_freq > est_target)
                                ? base_freq - est_target
                                : est_target - base_freq;

            if (error < best_error) {
                best_error = error;
                best_interval = interval;
                best_tac = tac_values[i];
                tickrate = div16_8(base_freq,interval);

                // Optional: break early if very close
                if (error == 0) goto done;
            }
        }
    }

done:
    tickinterval = best_interval;
    tac_setting = best_tac;

    TAC_REG = 0x00;
    TMA_REG = 256 - tickinterval;
    TAC_REG = tac_setting;
}

