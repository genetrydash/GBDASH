#include "register.h"
#include "macros.h"
#include "audio_driver.h"
#include "heartbeat.h"
#include <stdio.h>

static void tick(void) {
    tickmacros();
    setpitch(SOUND_PULSE1,getvalue(0));
}

static void init(void) {
    setenable(1);
    setmastervolume(7,7);
    setpan(SOUND_PULSE1,CHANNEL_BOTH);
    lfomacro(0,500,0,30,LFO_SAW,0);
    startmacro(0);
    setvolume(SOUND_PULSE1,15);

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