#include "physics.h"
#include <gb/gb.h>

unsigned int player_x = 0;      // Player's X position on screen
unsigned int player_y = 0; // Player's Y position on screen
int velocity_y = 0;       // Player's vertical speed
int frame = 0;
int x_pos = 0;
int y_pos = GROUND_Y * MODIFIER; 

void update_player() {
    // Apply gravity
    velocity_y += GRAVITY;
    // Apply vertical velocity
    y_pos += velocity_y;

    // Prevent the player from falling below the ground
    if (isonground()) {
        // Move up one subpixel until you're no longer colliding—but cap it
        do {
            y_pos -= 1;
            player_x = x_pos / MODIFIER;
    player_y = y_pos / MODIFIER;
        } while (isonground() && y_pos > 0);
        // Make sure you sit exactly on the ground
        y_pos += 1;
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

void jump() {
    if (isonground()) { // Only allow jumping if on the ground
        velocity_y = -JUMP_STRENGTH;
    }
}

int isonground() {
    if (y_pos >= GROUND_Y * MODIFIER) {
        return TRUE;
    }

    // Convert screen-relative player position to world tile coordinates
    unsigned char world_x = (SCX_REG + player_x) / 8;
    unsigned char world_y = (SCY_REG + player_y) + 7 / 8; // Just below the player

    unsigned char tile = get_bkg_tile_xy(world_x, world_y);

    // Solid tiles range from 0x30 to 0x100 (from 0x1030 to 0x1100 in memory)
    if (tile == 0x10) {
        return TRUE;
    }

    return FALSE;
}
