#pragma once
#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct Particle {
    Vector2 position;  // Current position
    // Vector2 velocity;  // Movement direction and speed
    float velocity;
    float lifetime;    // Time before the particle disappears
    // float size;        // Size of the particle
    Color color;       // Particle color
} Particle;

void gamePlay(void);
void update(void);
