#if !defined(DRIVER_H)
#define DRIVER_H

//Wavetable
void synthtick();
void setwavetable1(const unsigned char*);
void setwavetable2(const unsigned char*);
void setwavesynthparams(unsigned char effect, unsigned char amount, unsigned char power, unsigned char enable, unsigned char ratedivider, unsigned char speed);

typedef enum {
    INVERT,
    ADD,
    SUBTRACT,
    AVERAGE,
    PHASE,
    CHORUS,
    WIPE,
    FADE,
    PING_PONG,
    OVERLAY,
    NEGATIVE_OVERLAY,
    SLIDE,
    MIX,
    PHASE_MOD
} WaveEffect;

//Instrument Macros
typedef enum {
    SEQUENCE,
    ADSR,
    LFO
} MacroType;

#endif // DRIVER_H
