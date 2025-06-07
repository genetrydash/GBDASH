#include "register.h"
#include "audio_driver.h"
#include "heartbeat.h"
#include "macros.h"
#include "notes.h"

unsigned char wave_data[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};
int currentbeat, divc = 0;

static void beat(void)
{
    startmacro(0);
    currentbeat++;
}

static void tick(void) {
    
    if (divc%6==0) {
        tickmacros();
        setpitch(SOUND_WAVE,getvalue(0));
    }
    divc++;
}

static void init(void) {
    currentbeat = 0;
    divc = 6;
    setenable(1);
    setmastervolume(7, 7);
    setpan(SOUND_WAVE, CHANNEL_BOTH);

    setwave(wave_data);
    setvolume(SOUND_WAVE, 1);

    adsrmacro(0,pitch_table[C_6],pitch_table[C_3],255,0,10,0,0,0,255);

    setbeatcallback(beat);
    settickcallback(tick);
    settickrate(360);
    setbpm(120);
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