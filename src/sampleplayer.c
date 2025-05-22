#include <stdint.h>
#include <stdio.h>
#include <gb/gb.h>
#include "sampleplayer.h"

uint8_t *sample = 0;
uint8_t current = 0;
uint8_t playing = 0;
uint8_t j = 0;

void setsample(uint8_t *new_sample)
{
    sample = new_sample;
}

void initsample()
{
    current = 0;
}

void update()
{
    
    for (j = 0; j < 16; j++)
    {
        _AUD3WAVERAM[j] = sample[current + j];
    }
    current += 16;
}