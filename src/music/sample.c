#include "register.h"
#include "audio_driver.h"
#include "heartbeat.h"
#include "macros.h"
#include "notes.h"

unsigned char wave_data[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
int currentbeat, divc = 0;

static void beat(void)
{
    startmacro(0);
    currentbeat++;
}

static void tick() {
    divc++;
    if (divc%6==0) {
        tickmacros();
    setpitch(SOUND_WAVE,getvalue(0));
    }
    
}

static void init(void) {
    currentbeat = 0;
    divc = 6;
    setenable(1);
    setmastervolume(7, 7);
    setpan(SOUND_WAVE, CHANNEL_BOTH);

    setwave(wave_data);
    setvolume(SOUND_WAVE, 1);

    adsrmacro(0,C5,C4,255,0,20,0,0,0,255);

    setbeatcallback(beat);
    settickcallback(tick);
    settickrate(360);
    setbpm(170);
    initirq();
}

static void stopandclean(void) {
    setenable(0);
    setbeatcallback(0);
    settickcallback(0);
    closeirq();
}

void register1(void) {
    registermodule("Sample",init,stopandclean,"None");
}