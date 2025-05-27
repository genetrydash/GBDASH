#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "physics.h"
#include "register.h"

extern void register_all_music(void);

void do_scroll(void)
{
    while (x_pos > (50 << MODIFIER_SHIFT))
    {
        scroll_bkg(1, 0); // Scroll if player is over 1/3 of the screen
        x_pos -= (1 << MODIFIER_SHIFT);
    }
}



void main(void)
{
    char current_track = 1;

    register_all_music();
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_sprite_data(0, 2, TileLabel);
    set_sprite_tile(0, 0);

    play(current_track);

    while (1)
    {
        uint8_t joy = joypad();

        if (joy & J_A || joy & J_UP) {
            jump();
        }

        // 🔄 Change music on SELECT
        if (joy & J_SELECT) {
            stop(current_track);      // Stop current music
            current_track++;

            // Wrap around if the next one doesn’t exist
            while (!exists(current_track)) {
                current_track = 0;
            }

            play(current_track);      // Play next one

            // Wait for button release to avoid instant repeat
            while (joypad() & J_SELECT) wait_vbl_done();
        }

        do_scroll();
        update_player();
        move_sprite(0, player_x, player_y);

        wait_vbl_done();
    }
}