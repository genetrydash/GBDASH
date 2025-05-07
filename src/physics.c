#include "physics.h"

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

void jump() {
    if (player_y == GROUND_Y) { // Only allow jumping if on the ground
        velocity_y = -JUMP_STRENGTH;
    }
}