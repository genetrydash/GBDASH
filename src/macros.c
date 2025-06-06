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
    uint8_t atk, hld, dcy, sus, sus_t, sus_d, rel;

} ADSRMacro;

typedef struct
{
    uint8_t speed;
    lfowave wave;
    int16_t phase;
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

    int value;
    int16_t pos;
    int16_t ival;
    int top;
    int bottom;
    int8_t divcnt;

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
    if (ID >= MAX_MACROS)
        return;

    macros[ID].type = MACRO_SEQUENCE;
    macros[ID].active = false; // initially inactive until started
    macros[ID].delay = 0;
    macros[ID].step = 0;

    SequenceMacro *seq = &macros[ID].data.sequence;

    // Copy the sequence values, limit length to max 16
    if (length > 16)
        length = 16;
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
    if (ID >= MAX_MACROS)
        return;

    macros[ID].type = MACRO_ADSR;
    macros[ID].active = false;
    macros[ID].delay = 0;
    macros[ID].step = 0;

    ADSRMacro *adsr = &macros[ID].data.adsr;

    macros[ID].top = top;
    macros[ID].bottom = bottom;
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
    if (ID >= MAX_MACROS)
        return;

    macros[ID].type = MACRO_LFO;
    macros[ID].active = false;
    macros[ID].delay = 0;
    macros[ID].step = 0;

    LFOMacro *lfo = &macros[ID].data.lfo;

    macros[ID].top = top;
    macros[ID].bottom = bottom;
    lfo->speed = speed;
    lfo->wave = wave;
    lfo->phase = phase;
}

void globalparams(uint8_t ID, uint8_t delay, uint8_t step)
{
    if (ID >= MAX_MACROS)
        return;

    macros[ID].delay = delay;
    macros[ID].step = step;
}

void startmacro(uint8_t ID)
{
    macros[ID].ival = 0;
    macros[ID].pos = 0;
    macros[ID].active = true;
    macros[ID].release = false;
}

void stopmacro(uint8_t ID)
{
    macros[ID].active = false;
}

void releasemacro(uint8_t ID)
{
    macros[ID].release = true;
}

void tickmacro(uint8_t ID)
{
    if (ID > MAX_MACROS)
        return;
    Macro *macro = &macros[ID];
    ADSRMacro *adsr = &macro->data.adsr;
    SequenceMacro *sequence = &macro->data.sequence;
    LFOMacro *lfo = &macro->data.lfo;
    uint16_t Pos = 0;
    char sloopvalid = sequence->looppoint < sequence->length;
    char srelvalid = sequence->releasepoint < sequence->length;
    switch (macro->type)
    {
    case MACRO_SEQUENCE:
        Pos = macro->pos;
        if (Pos > sequence->length || Pos > sequence->releasepoint)
        {
            if (!sloopvalid && srelvalid && !macro->release)
            {
                Pos = sequence->releasepoint;
            }
            else if (sloopvalid && !macro->release)
            {
                Pos = sequence->looppoint;
            }
            else if (Pos >= sequence->length)
            {
                macro->active = false;
            }
        } else {
            Pos++;
        }
        
        macro->value = sequence->sequence[Pos];
        macro->pos = Pos;
        break;
    case MACRO_LFO:
        Pos = (macro->pos + lfo->phase);
        Pos &= 1023;
        switch (lfo->wave)
        {
        case LFO_TRI:
            macro->ival = ((Pos & 512) ? (1023 - Pos) : (Pos)) >> 1;
            break;
        case LFO_SAW:
            macro->ival = Pos >> 2;
            break;
        case LFO_PUL:
            macro->ival = (Pos & 512) ? 255 : 0;
            break;
        default:
            break;
        }

        macro->value = macro->bottom + (int16_t)(((int32_t)(macro->top - macro->bottom) * macro->ival) >> 8);

        macro->pos += lfo->speed;
        macro->pos &= 1023;
        break;
    case MACRO_ADSR:
        if (macro->pos != 5 && macro->release)
            macro->pos = 5;
        switch (macro->pos) // pos is used as stage.
        {
        case 0: // Attack
            macro->ival += adsr->atk;
            if (macro->ival >= 255)
            {
                macro->ival = 255;
                macro->pos = 1;
                macro->divcnt = adsr->hld;
            }
            break;
        case 1: // Hold
            macro->divcnt--;
            if (macro->divcnt == 0)
            {
                macro->pos = 2;
            }
            break;
        case 2: // Decay
            macro->ival -= adsr->dcy;
            if (macro->ival > 255 || macro->ival <= adsr->sus)
            {
                macro->ival = adsr->sus;
                macro->divcnt = adsr->sus_t;
                macro->pos = 3;
            }
            break;
        case 3: // Sustain
            macro->divcnt--;
            if (macro->divcnt == 0)
            {
                macro->pos = 4;
            }
            break;
        case 4: // Decay After Sustain
            macro->ival -= adsr->sus_d;
            if (macro->ival > 255)
            {
                macro->ival = 0;
                macro->pos = 6;
            }
            break;
        case 5: // Release
            macro->ival -= adsr->rel;
            if (macro->ival > 255)
            {
                macro->ival = 0;
                macro->pos = 6;
            }
            break;
        case 6: // End
            macro->active = false;
            break;
        default:
            break;
        }
        macro->value = macro->bottom + (int16_t)(((int32_t)(macro->top - macro->bottom) * macro->ival) >> 8);
        break;
    default:
        break;
    }
}

void tickmacros(void)
{
    uint8_t i;
    for (i = 0; i < MAX_MACROS; i++)
    {
        if (macros[i].active)
        {
            tickmacro(i);
        }
    }
}

int getvalue(uint8_t ID)
{
    return macros[ID].value;
}