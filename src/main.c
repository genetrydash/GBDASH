#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "physics.h"
#include "sampleplayer.h"
#include "audio_driver.h"
#include "music.h"

void do_scroll()
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
unsigned char wave_data[32] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15};

void main()
{
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);
    // music_init();

    setwave(wave_data);
    setenable(1);
    setmastervolume(7, 7);
    setpan(SOUND_PULSE1, CHANNEL_BOTH);
    setpan(SOUND_WAVE, CHANNEL_BOTH);
    setlengthandduty(SOUND_PULSE1, 64, 2);
    setlengthandduty(SOUND_WAVE, 255, 0);
    

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_sprite_data(0, 2, TileLabel);
    set_sprite_tile(0, 0);

    int pan_step = 0; // Range: 0 to 7
    char pan_dir = 0; // 1 = right, 0 = left
    char left, right;
    setfreq(SOUND_PULSE1, 262);
    setsweep(SWEEP_DOWN, 3, 1);
    setenvolope(SOUND_PULSE1, 0, 15, 1);
    setvolume(SOUND_WAVE, 1);
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
        update();

        if (current_frame % 24 == 0)
        {
            setsweep(SWEEP_DOWN, 3, 1);
            setlengthandduty(SOUND_PULSE1, 64, 2);
            setfreq(SOUND_PULSE1, 262);
            setenvolope(SOUND_PULSE1, 0, 15, 1);
        }

        if ((current_frame + 12) % 24 == 0)
        {
            setenvolope(SOUND_PULSE1, 0, 15, 1);
            setlengthandduty(SOUND_PULSE1, 64, 1);
            setsweep(SWEEP_DOWN, 0, 0);
            if ((current_frame + 12) % 96 == 0)
            {
                if ((current_frame) % 768 >= 384)
                {
                    setfreq(SOUND_PULSE1, 117);
                }
                else
                {
                    setfreq(SOUND_PULSE1, 138);
                }
            }
            else if ((current_frame + 12) % 768 >= 384)
            {
                setfreq(SOUND_PULSE1, 104);
            }
            else
            {
                setfreq(SOUND_PULSE1, 130);
            }
        }
        if (current_frame % 768 == 384)
        {
            setwave(wave_data);
            setfreq(SOUND_WAVE, 130);
        }
        else if (current_frame % 768 == 0)
        {
            setwave(wave_data);
            setfreq(SOUND_WAVE, 103);
        }

        wait_vbl_done();
        current_frame += 1;
    }
}
