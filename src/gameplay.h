#pragma once
#include "raylib.h"
#include "titlescreen.h"
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

typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float size;
    float rotation;
    float rotationSpeed;
    int asteroidVariant;
    int explosionVariant;
    int currentFrame;
    int frameSpeed;
    int frameCounter;
    int maxFrames;
    bool alive;
    bool fragment;
} Asteroid;

void initGamePlay(void);
void gamePlay(int* currScene, int* score);
void update(void);
void particleHandler(void);
void asteroidHandler(int* currScene, int* score);
void cleanup(void);