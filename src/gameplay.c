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

Particle shoot;
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

Shader shader;

    Texture2D sprite;
void gamePlay(void){
    if (init){
        shader = LoadShader(0, TextFormat("src/bg.fs", GLSL_VERSION));
        SetShaderValue(shader, GetShaderLocation(shader, "resolution"),
                   (float[2]){ 1280, 800}, SHADER_UNIFORM_VEC2);

        srand((unsigned int)time(NULL));
        spaceShip = (Vector2*)malloc(sizeof(Vector2));
        spaceShip->x = 400.0;
        spaceShip->y = 400.0;
        rotation = 0.0;
        speed = 0.0;
        init = false;

        sprite = LoadTexture("src/spaceship.png");
        for (int i = 0; i < numParticles; i++) {
            particles[i] = (Particle*)malloc(sizeof(Particle));
            //memset(particles[i],0,sizeof(Particle));
            particles[i]->velocity = (Vector2){0,0};
            particles[i]->lifetime = 0.0;
            particles[i]->position.x = 10;
            particles[i]->position.y = 10;
            particles[i]->alive = false;
        }
        shoot.position.x = 0;
        shoot.position.y = 0;
        shoot.velocity.x = 0;
        shoot.velocity.y = 0;
        shoot.lifetime = 0; 
        shoot.alive = false;
    }
    else{
        float time = (float)gameFrame*0.01;
        SetShaderValue(shader, GetShaderLocation(shader, "iTime"), &time, SHADER_UNIFORM_FLOAT);
        float x = (float)spaceShip->x*0.02;
        SetShaderValue(shader, GetShaderLocation(shader, "iMousex"), &x, SHADER_UNIFORM_FLOAT);
        float y = (float)spaceShip->y*0.02;
        SetShaderValue(shader, GetShaderLocation(shader, "iMousey"), &y, SHADER_UNIFORM_FLOAT);
        BeginDrawing();
        ClearBackground(BLACK);

        BeginShaderMode(shader);
        DrawRectangle(0, 0, 1280, 800, BLACK); // Full-screen shader effect
        EndShaderMode();

        //DrawPolyLinesEx(*spaceShip, 3, 40.0, rotation, 5.0, WHITE);
        //DrawTextureEx(sprite, *spaceShip,rotation, 0.25, WHITE);  // Draw a Texture2D with extended parameters


        if (shoot.alive){
            Vector2 end = {shoot.position.x +shoot.velocity.x, shoot.position.y + shoot.velocity.y};
            DrawLineEx(shoot.position, end, 5.0, WHITE);   
        }
        for (int i = 0; i < numParticles; i++) {
            if (particles[i]->alive)
            {
                Color color = {i % 255,(2*i) % 255,(3*i) % 255,255};
                DrawCircleV(particles[i]->position,5.0, color);
            }
        }
        Rectangle source = { 0.0f, 0.0f, sprite.width, sprite.height }; // Full texture
        Rectangle destination = { spaceShip->x, spaceShip->y, 0.25f*sprite.width, 0.25f*sprite.height }; // Destination rectangle
        Vector2 origin = { 0.25f*(sprite.width / 2.0f), 0.25f*(sprite.height / 2.0f)}; // Center of the sprite
        DrawTexturePro(sprite, source, destination,origin, rotation+90.0f, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters

        //printf("%d,%d,%d\n",particles[0]->color.r,particles[0]->color.g,particles[0]->color.b);
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
    if (IsKeyDown(KEY_UP) && speed < 0.15){
        speed += 0.05;
    }

    spaceShip->x += speed*((180/PI)*cosf((PI/180.0)*rotation));
    spaceShip->y += speed*((180/PI)*sinf((PI/180.0)*rotation));

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
    
    if (IsKeyDown(KEY_SPACE) && shoot.lifetime <= 0){
        const float fast = 40.0;
        float angle_rad = (PI / 180.0f) * rotation;
        shoot.position = *spaceShip;
        shoot.velocity.x = fast* (speed + 1)* cosf(angle_rad);
        shoot.velocity.y = fast * (speed + 1) * sinf(angle_rad);
        shoot.lifetime = 20;
        shoot.alive = true;
    }
    if (shoot.alive){
        shoot.position.x += shoot.velocity.x;
        shoot.position.y += shoot.velocity.y;

       shoot.lifetime -= 1;
        if (shoot.lifetime <= 0) {
            shoot.alive = false;
        }
    }

    //printf("Speed: %.2f\n", speed);
    if (IsKeyDown(KEY_UP) /*&& speed < 0.09*/ && (gameFrame%1==0))
    {
        for (int i = 0; i < 5; i++){
            float max_deviation = 30.0f * (PI / 180.0f); // 10 degrees in radians
            float random_deviation = (rand() / (float)RAND_MAX) * 2 * max_deviation - max_deviation;

            float fraction = 20.0;
            float angle_rad = (PI / 180.0f) * rotation + random_deviation;
            //printf("anglerad: %.2f\n", angle_rad);

            Particle *p = particles[nextParticleIndex];
            p->alive = true;
            p->velocity.x = -fraction * speed * cosf(angle_rad);
            p->velocity.y = -fraction * speed * sinf(angle_rad);
            //printf("Particle Velocity: x = %.2f, y = %.2f\n", -fraction * speed * cosf((PI / 180.0f) * rotation), -fraction * speed * sinf((PI / 180.0f) * rotation));

            //p->color = WHITE;
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

void cleanup(void){
    free(spaceShip);
	for(int i = 0; i < numParticles; i++){
		free(particles[i]);
	}
	UnloadShader(shader);
    UnloadTexture(sprite);
}
