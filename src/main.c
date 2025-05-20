#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "physics.h"

void do_scroll() {
    while (x_pos > 50 * MODIFIER) {
        scroll_bkg(1, 0);  // Scroll if player is over 1/3 of the screen
        x_pos -= 1 * MODIFIER;
    }
}

void main() {
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);
    //music_init();

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    set_sprite_data(0, 2, TileLabel);
    set_sprite_tile(0, 0);

    while (1) {
        if (joypad() & (J_A | J_UP)) {
            jump();
        }

        do_scroll();
        update_player();
            // Move sprite
    move_sprite(0, player_x, player_y);
        wait_vbl_done();
    }
}
