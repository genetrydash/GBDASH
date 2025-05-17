#pragma once

// Constants
#define SUBPIXEL_BITS 6
#define SUBPIXEL_MULTIPLIER (1 << SUBPIXEL_BITS) // 32
#define PIXELS_PER_BLOCK 8
#define BLOCKS_TO_VALUE (PIXELS_PER_BLOCK * SUBPIXEL_MULTIPLIER) // 256
#define GB_FRAMERATE 59.73
#define MINI_SIZE 0.6
#define SHIP_MINI_FACTOR 0.85

typedef enum {
    VEHICLE_CUBE,
    VEHICLE_SHIP
} VehicleType;

// ---- Cube physics (8-bit subpixels) ----

#define CUBE_GRAVITY 108
#define CUBE_MAX_Y_SPEED (7 * SUBPIXEL_MULTIPLIER)

#define CUBE_WIDTH 8
#define CUBE_HEIGHT 8
#define MINI_CUBE_WIDTH  (CUBE_WIDTH * MINI_SIZE)
#define MINI_CUBE_HEIGHT (CUBE_HEIGHT * MINI_SIZE)

#define CUBE_FIRST_JUMP_HEIGHT   (2.125 * BLOCKS_TO_VALUE)
#define CUBE_FIRST_JUMP_SPEED    isqrt(2 * CUBE_GRAVITY * CUBE_FIRST_JUMP_HEIGHT)

#define CUBE_JUMP_HEIGHT         (2.1653 * BLOCKS_TO_VALUE)
#define CUBE_JUMP_SPEED          isqrt(2 * CUBE_GRAVITY * CUBE_JUMP_HEIGHT)

#define CUBE_MINI_JUMP_SPEED     (CUBE_JUMP_SPEED * 0.8)

#define CUBE_YELLOW_PAD_MAX_HEIGHT (4.2 * BLOCKS_TO_VALUE)
#define CUBE_YELLOW_PAD_JUMP_SPEED isqrt(2 * CUBE_GRAVITY * CUBE_YELLOW_PAD_MAX_HEIGHT)

// ---- Ship physics (8-bit subpixels) ----

#define SHIP_GRAVITY_BASE       42
#define SHIP_GRAVITY            (SHIP_GRAVITY_BASE * 0.8)
#define SHIP_GRAVITY_AFTER_HOLD (SHIP_GRAVITY_BASE * 1.2)

#define SHIP_WIDTH 16
#define SHIP_HEIGHT 16
#define MINI_SHIP_WIDTH  (SHIP_WIDTH * MINI_SIZE)
#define MINI_SHIP_HEIGHT (SHIP_HEIGHT * MINI_SIZE)

#define SHIP_MAX_Y_SPEED         (6.4f * (16.0f / 32.0f) * SUBPIXEL_MULTIPLIER)
#define SHIP_MAX_Y_SPEED_HOLDING (8.0f * (16.0f / 32.0f) * SUBPIXEL_MULTIPLIER)
#define SHIP_MINI_MAX_Y_SPEED         (SHIP_MAX_Y_SPEED / SHIP_MINI_FACTOR)
#define SHIP_MINI_MAX_Y_SPEED_HOLDING (SHIP_MAX_Y_SPEED_HOLDING / SHIP_MINI_FACTOR)

#define SHIP_YELLOW_PAD_MAX_HEIGHT (2.4 * BLOCKS_TO_VALUE)
#define SHIP_YELLOW_PAD_JUMP_SPEED isqrt(2 * SHIP_GRAVITY_AFTER_HOLD * SHIP_YELLOW_PAD_MAX_HEIGHT)

// ---- Physics structure ----

typedef struct {
    float gravity;
    float jumpSpeed;
    float maxYSpeed;
    float width, height;
} Physics;

static Physics getPhysics(VehicleType vehicle, int isMini) {
    Physics p;

    if (vehicle == VEHICLE_CUBE) {
        p.gravity = CUBE_GRAVITY;
        p.jumpSpeed = isMini ? CUBE_MINI_JUMP_SPEED : CUBE_JUMP_SPEED;
        p.maxYSpeed = CUBE_MAX_Y_SPEED;
        p.width  = isMini ? MINI_CUBE_WIDTH : CUBE_WIDTH;
        p.height = isMini ? MINI_CUBE_HEIGHT : CUBE_HEIGHT;
    } else {
        float gravityBase = isMini ? (SHIP_GRAVITY_BASE / SHIP_MINI_FACTOR) : SHIP_GRAVITY_BASE;
        float gravity = gravityBase * 0.8;
        float jumpSpeed = isqrt(2 * (gravityBase * 1.2) * SHIP_YELLOW_PAD_MAX_HEIGHT);
        float maxYSpeed = isMini ? SHIP_MINI_MAX_Y_SPEED : SHIP_MAX_Y_SPEED;
        float width  = isMini ? MINI_SHIP_WIDTH : SHIP_WIDTH;
        float height = isMini ? MINI_SHIP_HEIGHT : SHIP_HEIGHT;

        p.gravity = gravity;
        p.jumpSpeed = jumpSpeed;
        p.maxYSpeed = maxYSpeed;
        p.width = width;
        p.height = height;
    }

    return p;
}
