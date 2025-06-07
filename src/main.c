#include <gb/gb.h>
#include <stdint.h>
#include "menuscreen.h"
#include "register.h"
#include "level.h"
#include "macros.h"


extern void register_all_music(void);

static void waitforframes(char frames) {
    while (frames > 0) {
        frames--;
        wait_vbl_done();
    }
}

void main(void)
{
    clearmacros();
    register_all_music();
    domenu();
    while (TRUE)
    {
        uint8_t joy = joypad();
        if (joy & J_START) {
            dolevel();
        }
    }
    
}