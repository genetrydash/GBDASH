#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "physics.h"
#include "audio_driver.h"
#include "heartbeat.h"

void do_scroll(void)
{
    while (x_pos > 50 * MODIFIER)
    {
        scroll_bkg(1, 0); // Scroll if player is over 1/3 of the screen
        x_pos -= 1 * MODIFIER;
    }
}

char p = 0;
int pan_step = 0; // 0..31
unsigned int current_frame = 0;
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

void main(void)
{
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);

    setenable(1);
    setmastervolume(7, 7);
    setpan(SOUND_PULSE1, CHANNEL_BOTH);
    setpan(SOUND_WAVE, CHANNEL_BOTH);
    setlengthandduty(SOUND_PULSE1, 64, 2);
    setlengthandduty(SOUND_PULSE2, 64, 1);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_sprite_data(0, 2, TileLabel);
    set_sprite_tile(0, 0);

    setfreq(SOUND_PULSE1, 262);
    setsweep(SWEEP_DOWN, 3, 1);
    setenvolope(SOUND_PULSE1, 0, 15, 1);
    setwave(wave_data);
    setvolume(SOUND_WAVE, 1);

    setbeatcallback(beat);
    settickrate(360);
    setbpm(200*2);
    initirq();

    while (1)
    {
        if (joypad() & (J_A | J_UP))
        {
            jump();
        }

        do_scroll();
        update_player();
        // Move sprite
        move_sprite(0, player_x, player_y);

        wait_vbl_done();
        current_frame += 1;
    }
}
