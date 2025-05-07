#ifndef PHYSICS_H
#define PHYSICS_H

#define GRAVITY 8
#define JUMP_STRENGTH 96

#define MOVE_SPEED_X 44 // Horizontal movement speed
#define MODIFIER 32

#define GROUND_Y 128  // Ground level (Y position on screen)

extern int x_pos;
extern int y_pos;
extern unsigned int player_y;
extern unsigned int player_x;

void jump();
void update_player();


#endif