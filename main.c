#include <gb/gb.h>
#include <stdio.h>
#include "tiles.c"

#define GRAVITY 1
#define JUMP_STRENGTH 10
#define GROUND_Y 120  // El nivel del suelo (posición Y en la pantalla)
#define MOVE_SPEED_X 2 // Velocidad de movimiento horizontal

UINT8 player_x = 50;      // Posición X del jugador en la pantalla
UINT8 player_y = GROUND_Y; // Posición Y del jugador en la pantalla
INT8 velocity_y = 0;       // Velocidad vertical del jugador

void update_player_position() {
    // Actualizamos la velocidad vertical con la gravedad
    velocity_y += GRAVITY;

    // Actualizamos la posición Y del jugador según la velocidad vertical
    player_y += velocity_y;

    // Evitamos que el jugador pase por debajo del nivel del suelo
    if (player_y >= GROUND_Y) {
        player_y = GROUND_Y;
        velocity_y = 0; // Detenemos la velocidad hacia abajo cuando el jugador toca el suelo
    }
    
    // Actualizamos la posición del sprite del jugador
    move_sprite(0, player_x, player_y);
}

void jump() {
    if (player_y == GROUND_Y) { // Solo permitimos saltar si estamos en el suelo
        velocity_y = -JUMP_STRENGTH; // Aplicamos la velocidad vertical hacia arriba para el salto
    }
}

void move_player_auto() {
    // Movimiento automático en la dirección X (hacia la derecha)
    player_x += MOVE_SPEED_X;

    // Si el jugador se sale de la pantalla (por ejemplo, al superar el borde derecho), lo reiniciamos
    if (player_x > 160) {  // El ancho de la pantalla es 160 píxeles
        player_x = 0;
    }

    // Actualizamos la posición del sprite
    move_sprite(0, player_x, player_y);
}

void main() {
    // Inicializamos la Game Boy
    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
    
    // Cargamos el sprite del jugador en memoria (usamos la ranura de sprite 0)
    set_sprite_data(0, 1, player_sprite);
    set_sprite_tile(0, 0);
    
    while(1) {
        // Actualizamos el movimiento automático del jugador
        move_player_auto();
        
        // Comprobamos si se presiona el botón de salto
        if (joypad() & J_A) {
            jump();
        }
        
        // Actualizamos la posición del jugador (gravedad y salto)
        update_player_position();
        
        // Esperamos al siguiente fotograma
        wait_vbl_done();
    }
}