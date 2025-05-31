#include "macros.h"

typedef struct
{
    int sequence[16];
    uint8_t length;
    uint8_t looppoint;
    uint8_t releasepoint;
} SequenceMacro;

typedef struct
{
    int top;
    int bottom;
    uint8_t atk, hld, dcy, sus, sus_t, sus_d, rel;
} ADSRMacro;

typedef struct
{
    int top;
    int bottom;
    uint8_t speed;
    lfowave wave;
    uint16_t phase;
} LFOMacro;

typedef enum
{
    MACRO_NONE,
    MACRO_SEQUENCE,
    MACRO_ADSR,
    MACRO_LFO
} MacroType;

typedef struct
{
    MacroType type;
    bool active;

    union
    {
        SequenceMacro sequence;
        ADSRMacro adsr;
        LFOMacro lfo;
    } data;

    uint8_t delay;
    uint8_t step;
} Macro;

#define MAX_MACROS 20
Macro macros[MAX_MACROS];

void clearmacros(void)
{
    uint16_t i;
    for (i = 0; i < MAX_MACROS; i++)
    {
        macros[i].active = false;
        macros[i].type = MACRO_NONE;
        macros[i].delay = 0;
        macros[i].step = 0;
    }
}

void sequencemacro(uint16_t ID, const int *sequence, uint8_t length, uint8_t looppoint, uint8_t releasepoint)
{
}

void adsrmacro(uint16_t ID, int top, int bottom,
               uint8_t atk, uint8_t hld, uint8_t dcy,
               uint8_t sus, uint8_t sus_t, uint8_t sus_d, uint8_t rel)
{
}

void lfomacro(uint16_t ID, int top, int bottom, uint8_t speed, lfowave wave, uint16_t phase)
{
}

void globalparams(uint16_t ID, uint8_t delay, uint8_t step)
{
}

void startmacro(uint16_t ID, int *value)
{
}

void stopmacro(uint16_t ID)
{
}

void releasemacro(uint16_t ID)
{
}
void tickmacros(void)
{
}
