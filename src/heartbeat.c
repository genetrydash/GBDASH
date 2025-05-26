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
const unsigned long timer_freqs[] = {4096, 16384, 65536, 262144};
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

// Set the tick rate as close as possible to `rate`
void settickrate(unsigned int rate) {
    unsigned char best_interval = 1;
    unsigned char best_tac = 0x04;
    unsigned long best_error = 0xFFFFFFFF;
    unsigned char i;
    for (i = 0; i < 4; i++) {
        unsigned long base_freq = timer_freqs[i];
        unsigned int interval;
        for (interval = 1; interval < 256; interval++) {
            unsigned long actual_rate = base_freq / interval;
            unsigned long error = (rate > actual_rate) ? rate - actual_rate : actual_rate - rate;

            if (error < best_error) {
                best_error = error;
                best_interval = interval;
                best_tac = tac_values[i];
                tickrate = actual_rate;
            }
        }
    }

    tickinterval = best_interval;
    tac_setting = best_tac;

    disable_interrupts();
    TAC_REG = 0x00;
    TMA_REG = 256 - tickinterval;
    TAC_REG = tac_setting;
    enable_interrupts();
}
