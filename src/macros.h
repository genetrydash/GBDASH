#ifndef MACROS_H
#define MACROS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LFO_TRI,
    LFO_SAW,
    LFO_PUL
} lfowave;

// Clear all macros to inactive state
void clearmacros(void);

// Setup sequence macro
void sequencemacro(uint8_t ID, const int *sequence, uint8_t length, uint8_t looppoint, uint8_t releasepoint);

// Setup ADSR macro
void adsrmacro(uint8_t ID, int top, int bottom,
               uint8_t atk, uint8_t hld, uint8_t dcy,
               uint8_t sus, uint8_t sus_t, uint8_t sus_d, uint8_t rel);

// Setup LFO macro
void lfomacro(uint8_t ID, int top, int bottom, uint8_t speed, lfowave wave, uint16_t phase);

// Set global parameters for a macro
void globalparams(uint8_t ID, uint8_t delay, uint8_t step);

// Start macro with given ID and attach value pointer
void startmacro(uint8_t ID);

// Stop macro with given ID
void stopmacro(uint8_t ID);

// Release macro with given ID
void releasemacro(uint8_t ID);

// Process all active macros per tick
void tickmacros(void);

// Get Current Value of a macro
int getvalue(uint8_t ID);

#endif // MACROS_H
