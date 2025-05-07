#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "music.h"

#define GRAVITY 8
#define JUMP_STRENGTH 96
#define GROUND_Y 128  // Ground level (Y position on screen)
#define MOVE_SPEED_X 44 // Horizontal movement speed
#define MODIFIER 32

unsigned int player_x = 0;      // Player's X position on screen
unsigned int player_y = 0; // Player's Y position on screen
int velocity_y = 0;       // Player's vertical speed
int frame = 0;
int x_pos = 0;
int y_pos = GROUND_Y * MODIFIER;

void jump() {
    if (player_y == GROUND_Y) { // Only allow jumping if on the ground
        velocity_y = -JUMP_STRENGTH;
    }
}

void update_player() {
    // Apply gravity
    velocity_y += GRAVITY;
    // Apply vertical velocity
    y_pos += velocity_y;

    // Prevent the player from falling below the ground
    if (y_pos >= GROUND_Y * MODIFIER) {
        y_pos = GROUND_Y * MODIFIER;
        velocity_y = 0;
    }

    // Automatic horizontal movement
    x_pos += MOVE_SPEED_X;
    if (x_pos > 160 * MODIFIER) {
        x_pos = 0;
    }

    // Convert internal position to screen position
    player_x = x_pos / MODIFIER;
    player_y = y_pos / MODIFIER;
}

void do_scroll() {
    if (x_pos > 50 * MODIFIER) {
    while (x_pos > 50 * MODIFIER) {
        scroll_bkg(1,0);
        x_pos -= 1 * MODIFIER;
    }
}
}

void main() {
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);
    music_init();

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
    frame++;
        wait_vbl_done();
    }
}
