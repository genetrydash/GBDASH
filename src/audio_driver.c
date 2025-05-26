#include "audio_driver.h"

static int freq_to_timer(int freq_hz, char iswave)
{
    if (iswave)
    {
        if (freq_hz <= 0)
            return 2047; // silence
        int timer = (int)2048 - (131072 / (freq_hz*2));
        if (timer < 0)
            timer = 0;
        if (timer > 2047)
            timer = 2047;
        return timer;
    }
    else
    {
        if (freq_hz <= 0)
            return 2047; // silence
        int timer = (int)2048 - (131072 / freq_hz);
        if (timer < 0)
            timer = 0;
        if (timer > 2047)
            timer = 2047;
        return timer;
    }
}

void setenable(char enable)
{
    if (enable)
    {
        REG(0xFF26) |= 0b10000000;
    }
    else
    {
        REG(0xFF26) &= ~0b10000000;
    }
}

void setmastervolume(char left, char right)
{
    // Keep bits 7 and 3, set left volume (bits 4-6), right volume (bits 0-2)
    REG(0xFF24) = (REG(0xFF24) & 0b10001000) |
                  ((left & 0b111) << 4) |
                  (right & 0b111);
}

void setpan(Sound_Channel channel, Channel pan)
{
    REG(0xFF25) &= ~(0b00010001 << channel);
    REG(0xFF25) |= (pan << channel);
}

void setfreq(Sound_Channel channel, int freq_hz)
{
    int timer = freq_to_timer(freq_hz,(channel == SOUND_WAVE));
    switch (channel)
    {
    case 0:
        REG(0xFF13) = timer & 0xFF;                                 // NR13 - freq low bits
        REG(0xFF14) = (REG(0xFF14) & 0xF8) | ((timer >> 8) & 0x07); // NR14 bits 0-2 freq high bits
        break;
    case 1:
        REG(0xFF18) = timer & 0xFF;                                 // NR23 - freq low bits
        REG(0xFF19) = (REG(0xFF19) & 0xF8) | ((timer >> 8) & 0x07); // NR24 bits 0-2 freq high bits
        break;
    case 2:
        REG(0xFF1D) = timer & 0xFF;                                 // NR33 - freq low bits
        REG(0xFF1E) = (REG(0xFF1E) & 0xF8) | ((timer >> 8) & 0x07); // NR34 bits 0-2 freq high bits
        break;
    default:
        // unsupported channel
        break;
    }
}

void setpitch(Sound_Channel channel, int timer)
{
    switch (channel)
    {
    case 0:
        REG(0xFF13) = timer & 0xFF;                                 // NR13 - freq low bits
        REG(0xFF14) = (REG(0xFF14) & 0xF8) | ((timer >> 8) & 0x07); // NR14 bits 0-2 freq high bits
        break;
    case 1:
        REG(0xFF18) = timer & 0xFF;                                 // NR23 - freq low bits
        REG(0xFF19) = (REG(0xFF19) & 0xF8) | ((timer >> 8) & 0x07); // NR24 bits 0-2 freq high bits
        break;
    case 2:
        REG(0xFF1D) = timer & 0xFF;                                 // NR33 - freq low bits
        REG(0xFF1E) = (REG(0xFF1E) & 0xF8) | ((timer >> 8) & 0x07); // NR34 bits 0-2 freq high bits
        break;
    default:
        // unsupported channel
        break;
    }
}

void setenvolope(Sound_Channel channel, char direction, char vol, char period)
{
    switch (channel)
    {
    case SOUND_PULSE1:
        REG(0xFF12) = (vol & 0b1111) << 4 | (direction & 0b1) << 3 | period & 0b111;
        break;

    case SOUND_PULSE2:
        REG(0xFF17) = (vol & 0b1111) << 4 | (direction & 0b1) << 3 | period & 0b111;
        break;

    case SOUND_NOISE:
        REG(0xFF21) = (vol & 0b1111) << 4 | (direction & 0b1) << 3 | period & 0b111;
        break;

    default:
        break;
    }
}

void setlengthandduty(Sound_Channel channel, char length, char duty)
{
    switch (channel)
    {
    case SOUND_PULSE1:
        // NR11 (0xFF11): Duty (bits 6–7), Length (bits 0–5)
        REG(0xFF11) = ((duty & 0b11) << 6) | (length & 0b111111);
        break;

    case SOUND_PULSE2:
        // NR21 (0xFF16): Duty (bits 6–7), Length (bits 0–5)
        REG(0xFF16) = ((duty & 0b11) << 6) | (length & 0b111111);
        break;

    case SOUND_WAVE:
        // NR31 (0xFF1B): Only Length (entire 8 bits); no duty
        REG(0xFF1B) = length;
        break;

    case SOUND_NOISE:
        // NR41 (0xFF20): Only Length (6 bits)
        REG(0xFF20) = length & 0b111111;
        break;

    default:
        break;
    }
}

void setsweep(Sweep_Dir dir, char shift, char period)
{
    REG(0xFF10) = ((period & 0b111) << 4) | ((dir & 0b1) << 3) | (shift & 0b111);
}

void setvolume(Sound_Channel channel, char volume)
{
    switch (channel)
    {
    case SOUND_PULSE1:
        // NR14: preserve bits 0-5, update bit 6 (length enable) and bit 7 (trigger)
        REG(0xFF12) = (volume & 0b1111) << 4;
        break;

    case SOUND_PULSE2:
        // NR24
        REG(0xFF17) = (volume & 0b1111) << 4;
        break;

    case SOUND_WAVE:
        // NR34
        REG(0xFF1C) = (volume & 0b11) << 5;
        break;

    case SOUND_NOISE:
        // NR44
        REG(0xFF21) = (volume & 0b1111) << 4;
        break;

    default:
        break;
    }
}
int _i = 0;
void setwave(const unsigned char *samples_4bit)
{
    REG(0xFF1A) = 0;
    for (_i = 0; _i < 16; _i++)
    {
        // Each byte holds two 4-bit samples
        unsigned char high = samples_4bit[_i * 2] & 0x0F;    // First sample (high nibble)
        unsigned char low = samples_4bit[_i * 2 + 1] & 0x0F; // Second sample (low nibble)
        unsigned char packed = (high << 4) | low;

        REG(0xFF30 + _i) = packed;
    }
    REG(0xFF1A) = 0b10000000;
}
