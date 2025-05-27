#include "physics.h"
#include <gb/gb.h>

unsigned int player_x = 0;      // Player's X position on screen
unsigned int player_y = 0; // Player's Y position on screen
int16_t velocity_y = 0;       // Player's vertical speed
uint32_t x_pos = 0;
int16_t y_pos = GROUND_Y << MODIFIER_SHIFT; 
uint8_t scroll_x = 0;

void update_player(void) {
    // Apply gravity
    velocity_y += GRAVITY;
    // Apply vertical velocity
    y_pos += velocity_y;

    // Prevent the player from falling below the ground
    if (isonground()) {
        // Move up one subpixel until you're no longer colliding—but cap it
        do {
            y_pos -= 1;
        } while (isonground() && y_pos > 0);
        // Make sure you sit exactly on the ground
        y_pos += 1;
        velocity_y = 0;
        
    }

    // Automatic horizontal movement
    x_pos += MOVE_SPEED_X;
    /*
    if (x_pos >= (160 << MODIFIER_SHIFT)) {
        x_pos -= (160 << MODIFIER_SHIFT);
    }
    */

    if (x_pos >= NO_SCROLL_POS) {
        player_x = NO_SCROLL_POS >> MODIFIER_SHIFT;
        scroll_x = (x_pos - NO_SCROLL_POS) >> MODIFIER_SHIFT;
    } else {
        player_x = x_pos >> MODIFIER_SHIFT;
    }
    // Convert internal position to screen position
    player_y = y_pos >> MODIFIER_SHIFT;
    SCX_REG = scroll_x&0xff;
}

void jump(void) {
    if (isonground()) { // Only allow jumping if on the ground
        velocity_y = -JUMP_STRENGTH;
    }
}

int isonground(void) {
    if (y_pos >= (GROUND_Y << MODIFIER_SHIFT)) {
        return TRUE;
    }

    // Convert screen-relative player position to world tile coordinates
    unsigned char world_x = (SCX_REG + player_x) >> 3;
    unsigned char world_y = ((SCY_REG + player_y) + 8) >> 3; // Just below the player

    // NOTE: DO NOT EVER READ FROM VRAM, IT WAITS FOR HBLANK WHICH EQUALS...
    // **MORE SLOWDOWN!!!**
    // unsigned char tile = get_bkg_tile_xy(world_x, world_y);

    // Solid tiles range from 0x30 to 0x100 (from 0x1030 to 0x1100 in memory)
    //if (tile == 0x10) {
      //  return TRUE;
    //}

    return FALSE;
}
