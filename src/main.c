#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "tiles.c"
#include "tileset.c"
#include "map.c"
#include "music.h"

#define GRAVITY 1
#define JUMP_STRENGTH 7
#define GROUND_Y 128  // Ground level (Y position on screen)
#define MOVE_SPEED_X 1 // Horizontal movement speed

unsigned char player_x = 0;      // Player's X position on screen
unsigned char player_y = GROUND_Y; // Player's Y position on screen
char velocity_y = 0;       // Player's vertical speed
char frame = 0;

void update_player_position() {
// Update vertical speed with gravity
if (frame % 20 == 0) {
velocity_y += GRAVITY;
// Update player's Y position based on vertical speed
player_y += velocity_y;
}

// Prevent the player from going below ground level  
if (player_y >= GROUND_Y) {  
    player_y = GROUND_Y;  
    velocity_y = 0; // Stop downward speed when the player hits the ground  
}  

// Update the player's sprite position  
move_sprite(0, player_x, player_y);  
frame++;

}

void jump() {
if (player_y == GROUND_Y) { // Only allow jumping if on the ground
velocity_y = -JUMP_STRENGTH; // Apply upward vertical speed for the jump
}
}

void move_player_auto() {
// Automatic movement in the X direction (to the right)
if (frame % 8 == 0) {
player_x += MOVE_SPEED_X;}

// If the player goes off-screen (e.g. past the right edge), reset  
if (player_x > 160) {  // Screen width is 160 pixels  
    player_x = 0;  
}  

// Update the sprite position  
move_sprite(0, player_x, player_y);

}

void main() {

set_bkg_data(0,24,TileLabel1);  
set_bkg_tiles(0,0,64,16,MapLabel);  
music_init();  // Call the function that was previously in main()  

// Initialise the Game Boy  
DISPLAY_ON;  
SHOW_BKG;  
SHOW_SPRITES;  

// Load the player's sprite into memory (use sprite slot 0)  
set_sprite_data(0,2,TileLabel);  
set_sprite_tile(0,0);  

while(1) {  
    // Update automatic player movement  
    move_player_auto();  

    // Check if the jump button is pressed  
    if (joypad() & (J_A | J_UP)) {  
        jump();  
    }  

    // Update the player's position (gravity and jumping)  
    update_player_position();  

    }
}
