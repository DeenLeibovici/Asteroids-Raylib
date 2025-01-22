#pragma once
#include "raylib.h"
#include <stdlib.h>
#include <math.h>

typedef struct Particle {
    Vector2 position;  // Current position
    // Vector2 velocity;  // Movement direction and speed
    Vector2 velocity;
    int lifetime;    // Time before the particle disappears
    // float size;        // Size of the particle
    //Color color;       // Particle color
    bool alive;
} Particle;

void gamePlay(void);
void update(void);
void particleHandler(void);
