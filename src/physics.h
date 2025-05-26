#ifndef PHYSICS_H
#define PHYSICS_H

#define GRAVITY 16
#define JUMP_STRENGTH 192

#define MOVE_SPEED_X 88 // Horizontal movement speed
#define MODIFIER 64

#define GROUND_Y 128  // Ground level (Y position on screen)
#define MAX_SPEED (7 * MODIFIER)

extern int x_pos;
extern int y_pos;
extern unsigned int player_y;
extern unsigned int player_x;

void jump(void);
void update_player(void);
int isonground(void);


#endif