#include "register.h"
#include "macros.h"
#include "audio_driver.h"
#include "heartbeat.h"
#include "notes.h"
#include <stdio.h>
#include <gb/gb.h>

static void tick(void) {
    tickmacros();
    setlengthandduty(SOUND_PULSE1,0,getvalue(0));
    setpitch(SOUND_PULSE1,G4);
}

const char m [] = {1, 1, 2, 2, 3, 3, 2, 2};

static void init(void) {
    setenable(1);
    setmastervolume(7,7);
    setpan(SOUND_PULSE1,CHANNEL_BOTH);
    sequencemacro(0,m,9,0,255);
    startmacro(0);
    setvolume(SOUND_PULSE1,5);
    add_VBL(tick);
}

static void stopandclean(void) {
    settickcallback(0);
    closeirq();
}

void register3(void) {
    registermodule("Macro Test",init,stopandclean,"None");
}