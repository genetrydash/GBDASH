#include <gb/gb.h>
#include <stdio.h>
#include "tiles.c"

unsigned char plx;

void main()
{
    printf("HELLO WORLD");
    plx: 60;

    set_sprite_data(0,2,TileLabel);
    set_sprite_tile(0,0);
    SHOW_SPRITES;


    while(1)
    {
        ++plx;
        move_sprite(0,plx,70);
        delay(10);
    }
}
