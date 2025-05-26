#include "register.h"
#include "audio_driver.h"
#include "heartbeat.h"

unsigned char wave_data[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
int currentbeat = 0;

void beat(void)
{
    if (currentbeat % 2 == 0)
    {
        setsweep(SWEEP_DOWN, 3, 1);
        setlengthandduty(SOUND_PULSE1, 64, 2);
        setfreq(SOUND_PULSE1, 262);
        setenvolope(SOUND_PULSE1, 0, 15, 1);
    }

    if ((currentbeat + 1) % 2 == 0)
    {
        setenvolope(SOUND_PULSE1, 0, 15, 1);
        setlengthandduty(SOUND_PULSE1, 64, 1);
        setsweep(SWEEP_DOWN, 0, 0);
        if ((currentbeat + 1) % 4 == 0)
        {
            if ((currentbeat) % 32 >= 16)
            {
                setfreq(SOUND_PULSE1, 117);
            }
            else
            {
                setfreq(SOUND_PULSE1, 138);
            }
        }
        else if ((currentbeat + 1) % 32 >= 16)
        {
            setfreq(SOUND_PULSE1, 104);
        }
        else
        {
            setfreq(SOUND_PULSE1, 130);
        }
    }

    if ((currentbeat) % 32 == 16)
    {
        setfreq(SOUND_WAVE, 52);
    }
    else if ((currentbeat) % 32 == 0)
    {
        setfreq(SOUND_WAVE, 65);
    }
    currentbeat++;
}


void init(void) {
    setenable(1);
    setmastervolume(7, 7);
    setpan(SOUND_PULSE1, CHANNEL_BOTH);
    setpan(SOUND_WAVE, CHANNEL_BOTH);
    setlengthandduty(SOUND_PULSE1, 64, 2);
    setlengthandduty(SOUND_PULSE2, 64, 1);

    setfreq(SOUND_PULSE1, 262);
    setsweep(SWEEP_DOWN, 3, 1);
    setenvolope(SOUND_PULSE1, 0, 15, 1);
    setwave(wave_data);
    setvolume(SOUND_WAVE, 1);

    setbeatcallback(beat);
    settickrate(360);
    setbpm(200*2);
    initirq();
}

void stopandclean(void) {
    setenable(0);
}

void register1(void) {
    registermodule("sample",init,stopandclean);
}