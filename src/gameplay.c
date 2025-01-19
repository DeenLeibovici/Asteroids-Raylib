#include "gameplay.h"
bool init = true;
Vector2* spaceShip;
float rotation;
float speed;
Particle* particles[100];



void gamePlay(void){
    if (init){
        spaceShip = (Vector2*)malloc(sizeof(Vector2));
        spaceShip->x = 400.0;
        spaceShip->y = 400.0;
        rotation = 0.0;
        speed = 0.0;
        init = false;
        for (int i = 0; i < 100; i++) {
            particles[i] = malloc(sizeof(Particle));
            //memset(particles[i],0,sizeof(Particle));
            particles[i]->velocity = 0.0;
            particles[i]->color = WHITE;
            particles[i]->lifetime = 0.0;
            particles[i]->position = *spaceShip;
        }
    }
    else{
        BeginDrawing();
        ClearBackground(BLACK);
        DrawPolyLinesEx(*spaceShip, 3, 40.0, rotation, 5.0, WHITE);

        for (int i = 0; i < 100; i++) {
            if (particles[i]->lifetime > 0.1)
                DrawCircleV(particles[i]->position,5.0, particles[i]->color);
        }
        EndDrawing();
        update();
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
    if (IsKeyDown(KEY_UP)){
        if (speed < 0.1)
            speed += 0.01;
        //activate particles
        for (int i = 0; i < 100; i++){
            particles[i]->lifetime+=0.1;
            particles[i]->velocity += 0.01;
        }

    }

    spaceShip->x += speed*((180/PI)*cosf((PI/180.0)*rotation));
    spaceShip->y += speed*((180/PI)*sinf((PI/180.0)*rotation));

    for (int i = 0; i < 100; i++){
        particles[i]->position.x += particles[i]->velocity*((180/PI)*cosf((PI/180.0)*rotation));
        particles[i]->position.y += particles[i]->velocity*((180/PI)*sinf((PI/180.0)*rotation));
    }
    //printf("%f\n", particles[0]->velocity);

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

    for (int i = 0; i < 100; i++){
        if (particles[i]->lifetime > 0){
            particles[i]->lifetime-=0.01;
            
        }
        if (particles[i]->velocity > 0){
            particles[i]->velocity-= 0.001;
        }
    }
}
