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
    bool release;

    union
    {
        SequenceMacro sequence;
        ADSRMacro adsr;
        LFOMacro lfo;
    } data;

    uint8_t delay;
    uint8_t step;

    int *value;  // pointer to external int value controlled by this macro
    uint8_t pos;

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

void sequencemacro(uint8_t ID, const int *sequence, uint8_t length, uint8_t looppoint, uint8_t releasepoint)
{
    if (ID >= MAX_MACROS) return;

    macros[ID].type = MACRO_SEQUENCE;
    macros[ID].active = false; // initially inactive until started
    macros[ID].delay = 0;
    macros[ID].step = 0;
    

    SequenceMacro *seq = &macros[ID].data.sequence;

    // Copy the sequence values, limit length to max 16
    if (length > 16) length = 16;
    uint8_t i;
    for (i = 0; i < length; i++)
    {
        seq->sequence[i] = sequence[i];
    }
    seq->length = length;
    seq->looppoint = looppoint;
    seq->releasepoint = releasepoint;
}

void adsrmacro(uint8_t ID, int top, int bottom,
               uint8_t atk, uint8_t hld, uint8_t dcy,
               uint8_t sus, uint8_t sus_t, uint8_t sus_d, uint8_t rel)
{
    if (ID >= MAX_MACROS) return;

    macros[ID].type = MACRO_ADSR;
    macros[ID].active = false;
    macros[ID].delay = 0;
    macros[ID].step = 0;

    ADSRMacro *adsr = &macros[ID].data.adsr;

    adsr->top = top;
    adsr->bottom = bottom;
    adsr->atk = atk;
    adsr->hld = hld;
    adsr->dcy = dcy;
    adsr->sus = sus;
    adsr->sus_t = sus_t;
    adsr->sus_d = sus_d;
    adsr->rel = rel;
}

void lfomacro(uint8_t ID, int top, int bottom, uint8_t speed, lfowave wave, uint16_t phase)
{
    if (ID >= MAX_MACROS) return;

    macros[ID].type = MACRO_LFO;
    macros[ID].active = false;
    macros[ID].delay = 0;
    macros[ID].step = 0;

    LFOMacro *lfo = &macros[ID].data.lfo;

    lfo->top = top;
    lfo->bottom = bottom;
    lfo->speed = speed;
    lfo->wave = wave;
    lfo->phase = phase;
}

void globalparams(uint8_t ID, uint8_t delay, uint8_t step)
{
    if (ID >= MAX_MACROS) return;

    macros[ID].delay = delay;
    macros[ID].step = step;
}

void startmacro(uint8_t ID, int *value)
{
    macros[ID].value = value;
    macros[ID].active = true;
    macros[ID].release = false;
}

void stopmacro(uint8_t ID)
{
    macros[ID].value = 0;
    macros[ID].active = false;
}

void releasemacro(uint8_t ID)
{
    macros[ID].release = true;
}

void tickmacro(uint8_t ID) {

}

void tickmacros(void)
{
    uint8_t i;
    for (i=0;1<MAX_MACROS;i++) {
        if (macros[i].active) {
            tickmacro(i);
        }
    }
}



int getvalue(uint8_t ID) {
    if (ID >= MAX_MACROS) return 0;          // bounds check
    if (macros[ID].value == 0) return 0;  // avoid dereferencing NULL

    return *(macros[ID].value);              // dereference pointer to get value
}