#ifndef PHYSICS_H
#define PHYSICS_H
// PLEASE USE 8-BIT OR 16-BIT INTEGERS
// DO NOT USE INT (IT IS 32-BIT IN GBDK WHICH MEANS MORE SLOWDOWN!!!!)
#include <stdint.h>


#define GRAVITY 14
#define JUMP_STRENGTH 188

#define NO_SCROLL_POS (50 << MODIFIER_SHIFT)
#define MOVE_SPEED_X 82 // Horizontal movement speed
#define MODIFIER_SHIFT 6

#define GROUND_Y 128  // Ground level (Y position on screen)
#define MAX_SPEED (7 << MODIFIER_SHIFT)

extern uint32_t x_pos;
extern int16_t y_pos;
extern unsigned int player_y;
extern unsigned int player_x;

void jump(void);
void update_player(void);
int isonground(void);


#endif