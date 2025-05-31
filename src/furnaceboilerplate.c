#include "audio_driver.h"
#include <gb/gb.h>
// Instruments
//00: Lead
void intrh00(Channel channel) {
    //Hardware Sequence
    if (channel != SOUND_WAVE) {
        //Tick 0, Envelope
        setenvolope(channel,AUDENV_DOWN,15,2);
        setlengthandduty(channel, 63, 0);
    }
    //Macros
}

//01: Slidedown
void intrh01(Channel channel) {
    //Hardware Sequence
    if (channel != SOUND_WAVE) {
        //Tick 0, Envelope
        setenvolope(channel,AUDENV_DOWN,15,3);
        setlengthandduty(channel, 63, 0);
    }
    //Macros
}