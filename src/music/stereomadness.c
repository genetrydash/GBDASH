#include "audio_driver.h"
#include "register.h"
#include "heartbeat.h"
#include <gb/gb.h>
#include "macros.h"
#include "notes.h"

int row = 0;

const unsigned int p1p00 [] = {C_4,0x000a,0xeeee};

void step(void) {

}

static void init(void) {
    setenable(1);
    setmastervolume(7,7);
    settickrate(360);
    setbpm(160);
    add_VBL(tickmacros);
}

static void stopandclean(void) {
    closeirq();
}

void register3(void) {
    registermodule("Stereo Madness",init,stopandclean,"None");
}