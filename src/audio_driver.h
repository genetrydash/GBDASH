#if !defined(AUDIO_DRIVER_H)
#define AUDIO_DRIVER_H

#define REG(addr) (*(volatile unsigned char *)(addr))

typedef enum {
    CHANNEL_NONE = 0b00000000,
    CHANNEL_LEFT = 0b00010000,
    CHANNEL_RIGHT = 0b00000001,
    CHANNEL_BOTH = 0b00010001,
} Channel;

typedef enum {
    SOUND_PULSE1 = 0,
    SOUND_PULSE2,
    SOUND_WAVE,
    SOUND_NOISE,
} Sound_Channel;

void setenable(char enable);
void setmastervolume(char left, char right);
void setpan(Sound_Channel channel, Channel pan);
void setfreq(Sound_Channel channel, int freq_hz);
void setenvolope(Sound_Channel channel, char direction, char vol, char period);
void setlengthandduty(Sound_Channel channel, char length, char duty);
void play(Sound_Channel channel, char use_length);

#endif // AUDIO_DRIVER_H
