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

void main()
{
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);
    //music_init();
    
    setenable(1);
    setmastervolume(7, 7);
    setpan(SOUND_PULSE1, CHANNEL_BOTH);
    setpan(SOUND_PULSE2, CHANNEL_BOTH);
    setlengthandduty(SOUND_PULSE1, 64, 2);
    setlengthandduty(SOUND_PULSE2, 64, 1);
    setfreq(SOUND_PULSE1, 110);
    // play(SOUND_PULSE1, 0);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    
    set_sprite_data(0, 2, TileLabel);
    set_sprite_tile(0, 0);
    
    int pan_step = 0; // Range: 0 to 7
    char pan_dir = 0; // 1 = right, 0 = left
    char left, right;
    setfreq(SOUND_PULSE1, 262);
    setfreq(SOUND_PULSE2, 65);
    setsweep(SWEEP_DOWN,3,1);
    setenvolope(SOUND_PULSE1,0,15,1);
    setenvolope(SOUND_PULSE2,0,15,2);
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
            setfreq(SOUND_PULSE1, 262);
            setsweep(SWEEP_DOWN,3,1);
            setenvolope(SOUND_PULSE1,0,15,1);
        }

        if ((current_frame + 12) % 192 == 0)
        {
            setfreq(SOUND_PULSE1, 262);
            setsweep(SWEEP_DOWN,3,1);
            setenvolope(SOUND_PULSE1,0,15,1);
        }

        if ((current_frame + 12) % 24 == 0)
        {
            setfreq(SOUND_PULSE2, 65);
            setenvolope(SOUND_PULSE2,0,15,2);
        }

        wait_vbl_done();
        current_frame += 1;
    }
}
