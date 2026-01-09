#pragma once
#include "raylib.h"
#include <stdlib.h>
#include <math.h>
// #define numParticles 1000
typedef struct Particle {
    Vector2 position;  // Current position
    // Vector2 velocity;  // Movement direction and speed
    Vector2 velocity;
    int lifetime;    // Time before the particle disappears
    // float size;        // Size of the particle
    //Color color;       // Particle color
    bool alive;
} Particle;

typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float size;
    float rotation;
    float rotationSpeed;
    int asteroidVariant;
    int currentFrame;
    int frameSpeed;
    int frameCounter;
    int maxFrames;
    bool alive;
} Asteroid;

void gamePlay(void);
void update(void);
void particleHandler(void);
void asteroidHandler(void);
void cleanup(void);
