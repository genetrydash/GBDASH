#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "music.h"

#define GRAVITY 1
#define JUMP_STRENGTH 7
#define GROUND_Y 128
#define MOVE_SPEED_X 1

int16_t player_x = 0;
int16_t player_y = GROUND_Y;
int8_t velocity_y = 0;

void update_player_position() {
    // Aplicar gravedad
    velocity_y += GRAVITY;
    player_y += velocity_y;

    // Evitar que caiga por debajo del suelo
    if (player_y >= GROUND_Y) {
        player_y = GROUND_Y;
        velocity_y = 0;
    }

    // Actualizar posición del sprite
    move_sprite(0, (uint8_t)player_x, (uint8_t)player_y);
}

void jump() {
    if (player_y == GROUND_Y) {
        velocity_y = -JUMP_STRENGTH;
    }
}

void move_player_auto() {
    player_x += MOVE_SPEED_X;

    if (player_x > 160) {
        player_x = 0;
    }

    move_sprite(0, (uint8_t)player_x, (uint8_t)player_y);
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
        move_player_auto();

        if (joypad() & (J_A | J_UP)) {
            jump();
        }

        update_player_position();

        delay(2);  // Reduce la velocidad general para mayor fluidez
    }
}
