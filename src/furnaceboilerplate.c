#include "audio_driver.h"
#include <gb/gb.h>
#include "macros.h"
#include "driverhelper.h"

// Instruments
//00: Lead
static void intrh00(Channel channel) {
    //Hardware Sequence
    if (channel != SOUND_WAVE) {
        //Tick 0, Envelope
        setenvolope(channel,AUDENV_DOWN,15,2);
        setlengthandduty(channel, 63, 0);
    }
    //Macros
    clearmacrosforchannel(channel);
}

//01: Slidedown
static void intrh01(Channel channel) {
    //Hardware Sequence
    if (channel != SOUND_WAVE) {
        //Tick 0, Envelope
        setenvolope(channel,AUDENV_DOWN,15,3);
        setlengthandduty(channel, 63, 0);
    }
    //Macros
    clearmacrosforchannel(channel);
}

//02: Duty Cycle String
static void intrh02(Channel channel) {
    //Hardware Sequence
    if (channel != SOUND_WAVE) {
        //Tick 0, Envelope
        setenvolope(channel,AUDENV_UP,5,0);
        setlengthandduty(channel, 63, 0);
    }
    //Macros
    clearmacrosforchannel(channel);
    //Duty/Noise
    const char d02 [] = {1, 1, 2, 2, 3, 3, 2, 2};
    if (channel < 2 || channel == SOUND_NOISE) {

    };
}