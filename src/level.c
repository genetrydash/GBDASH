#include <gb/gb.h>
#include <stdint.h>
#include "menuscreen.h"
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "physics.h"
#include "level.h"
#include "register.h"


static void setup(void)
{
    stopall();
    set_bkg_data(0, 24, TileLabel1);
    set_bkg_tiles(0, 0, 64, 16, MapLabel);

    SHOW_BKG;
    set_sprite_data(0, 2, TileLabel);
    set_sprite_tile(0, 0);
    player_x = 0;
    player_y = 0;
    x_pos = 0;
    y_pos = GROUND_Y << MODIFIER_SHIFT; 
    scroll_x = 0;
    move_sprite(0, player_x, player_y);
    SHOW_SPRITES;
    play(1);
}

void dolevel(void)
{
    fade(setup);

    while (1)
    {
        uint8_t joy = joypad();

        if (joy & J_A || joy & J_UP)
        {
            jump();
        }
        if (joy & J_START)
        {
            HIDE_SPRITES;
            domenu();
            break;
        }

        update_player();
        move_sprite(0, player_x, player_y);
        wait_vbl_done();
    }
}