#include "register.h"
#include "macros.h"
#include "audio_driver.h"
#include "heartbeat.h"
#include <stdio.h>

static void tick(void) {
    tickmacros();
    setpitch(SOUND_WAVE,getvalue(0));
    setlengthandduty(SOUND_WAVE,0,2);
}

const unsigned char wave[32] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
};

static void init(void) {
    setenable(1);
    setmastervolume(7,7);
    setpan(SOUND_WAVE,CHANNEL_BOTH);
    lfomacro(0,2000,0,10,LFO_TRI,0);
    startmacro(0);
    setwave(wave);
    setvolume(SOUND_WAVE,1);
    settickrate(60);
    setbpm(60);
    settickcallback(tick);
    initirq();
}

static void stopandclean(void) {
    settickcallback(0);
    closeirq();
}

void register3(void) {
    registermodule("Macro Test",init,stopandclean,"None");
}