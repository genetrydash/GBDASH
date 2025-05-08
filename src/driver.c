#include "driver.h"
#include <gb/hardware.h>
#include <gb/gb.h>

unsigned char effect, amount, power, enable, ratedivider, speed;
unsigned char* wave1, wave2, output;

void setwavesynthparams(unsigned char eff, unsigned char amt, unsigned char pwr, unsigned char en, unsigned char rate, unsigned char spd) {
    effect = eff;
    amount = amt;
    power = pwr;
    enable = en;
    ratedivider = rate;
    speed = spd;
}

void setwavetable1(const unsigned char* wavetable) {
    wave1 = wavetable;
}

void setwavetable2(const unsigned char* wavetable) {
    wave2 = wavetable;
}

void tick() {

}

void runeffect() {

}

int i = 0;
void updatewavetable(const unsigned char* wavetable) {
    // Wave RAM is 16 bytes at FF30–FF3F
    for (i = 0; i < 16; i++) {
        // Each byte holds two 4-bit samples: upper nibble and lower nibble
        // Pack two 4-bit samples into a single byte
        unsigned char packed = (wavetable[i * 2] << 4) | (wavetable[i * 2 + 1] & 0x0F);
        NR30_REG = NR30_REG & 0x7F; // Disable wave channel before writing (optional safety)
        *((unsigned char*)(0xFF30 + i)) = packed;
    }
}