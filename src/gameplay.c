#include "gameplay.h"
#include <stdio.h>
#include <time.h>
#define numParticles 1000
bool init = true;
Vector2* spaceShip;
float rotation;
float speed;
Particle* particles[numParticles];
unsigned int gameFrame = 0;
int nextParticleIndex = 0;

void gamePlay(void){
    if (init){
        srand((unsigned int)time(NULL));
        spaceShip = (Vector2*)malloc(sizeof(Vector2));
        spaceShip->x = 400.0;
        spaceShip->y = 400.0;
        rotation = 0.0;
        speed = 0.0;
        init = false;
        for (int i = 0; i < numParticles; i++) {
            particles[i] = (Particle*)malloc(sizeof(Particle));
            //memset(particles[i],0,sizeof(Particle));
            particles[i]->velocity = (Vector2){0,0};
            particles[i]->color = RED;
            particles[i]->lifetime = 0.0;
            particles[i]->position.x = 10;
            particles[i]->position.y = 10;
            particles[i]->alive = false;
        }
    }
    else{
        BeginDrawing();
        ClearBackground(BLACK);
        DrawPolyLinesEx(*spaceShip, 3, 40.0, rotation, 5.0, WHITE);

        for (int i = 0; i < numParticles; i++) {
            if (particles[i]->alive)
                DrawCircleV(particles[i]->position,5.0, RED);
        }
        EndDrawing();
        update();
        particleHandler();
        gameFrame++;
    }
}

void update(void){
    if (IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT)){
    }
    else if (IsKeyDown(KEY_RIGHT)){
        rotation += 2.0;
    }
    else if (IsKeyDown(KEY_LEFT)){
        rotation -= 2.0;
    }

    //Update Speed
    if (IsKeyDown(KEY_UP) && speed < 0.1){
        speed += 0.005;
    }

    spaceShip->x += speed*((180/PI)*cosf((PI/180.0)*rotation));
    spaceShip->y += speed*((180/PI)*sinf((PI/180.0)*rotation));

    //for (int i = 0; i < 100; i++){
    //    particles[i]->position.x += particles[i]->velocity*((180/PI)*cosf((PI/180.0)*rotation));
    //    particles[i]->position.y += particles[i]->velocity*((180/PI)*sinf((PI/180.0)*rotation));
    //}

    if (spaceShip->x > 1280)
        spaceShip->x = 0;
    else if (spaceShip->x < 0)
        spaceShip->x = 1280;

    if (spaceShip->y > 800)
        spaceShip->y = 0;
    else if (spaceShip->y < 0)
        spaceShip->y = 800;
    

    if (speed > 0)
        speed -= 0.001;
}

void particleHandler(void)
{
    //printf("Speed: %.2f\n", speed);
    if (IsKeyDown(KEY_UP) /*&& speed < 0.09*/ && (gameFrame%1==0))
    {
        for (int i = 0; i < 10; i++){
            float max_deviation = 30.0f * (PI / 180.0f); // 10 degrees in radians
            float random_deviation = (rand() / (float)RAND_MAX) * 2 * max_deviation - max_deviation;

            float fraction = 20.0;
            float angle_rad = (PI / 180.0f) * rotation + random_deviation;
            printf("anglerad: %.2f\n", angle_rad);

            Particle *p = particles[nextParticleIndex];
            p->alive = true;
            p->velocity.x = -fraction * speed * cosf(angle_rad);
            p->velocity.y = -fraction * speed * sinf(angle_rad);
            printf("Particle Velocity: x = %.2f, y = %.2f\n", -fraction * speed * cosf((PI / 180.0f) * rotation), -fraction * speed * sinf((PI / 180.0f) * rotation));

            p->color = WHITE;
            p->lifetime = 100;
            p->position = *spaceShip;

            nextParticleIndex = (nextParticleIndex + 1) % numParticles;
        }
    }
    for (int i = 0; i < numParticles; i++){
        if (particles[i]->alive){
            particles[i]->position.x += particles[i]->velocity.x;
            particles[i]->position.y += particles[i]->velocity.y;

            particles[i]->lifetime -= 1;
            if (particles[i]->lifetime <= 0) {
                particles[i]->alive = false;
            }
        }
    }
    
}
