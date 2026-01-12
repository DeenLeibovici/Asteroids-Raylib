#include "gameplay.h"
#include <stdio.h>
#include <time.h>
#define numParticles 1000
#define maxAsteroids 100

Vector2* spaceShip;
float rotation;
float speed;
Particle* particles[numParticles];
unsigned int gameFrame = 0;
int nextParticleIndex = 0;

Asteroid* asteroids[maxAsteroids];
Particle shoot;
#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

Shader shader;

Texture2D sprite;
Texture2D explosionSpriteSheet;
Texture2D asteroidSpriteSheet;

Sound shootSound;
Sound explosionSound;
Sound thrusterSound;;

void initGamePlay(void){
    //Initialization moved to gamePlay function
        //Load Shader
        #if defined(PLATFORM_WEB)
        shader = LoadShader(0, TextFormat("./assets/bgWeb.fs", GLSL_VERSION));
        SetShaderValue(shader, GetShaderLocation(shader, "resolution"),
                   (float[2]){ GetScreenWidth(), GetScreenHeight()}, SHADER_UNIFORM_VEC2);
        #else
        shader = LoadShader(0, TextFormat("./assets/bg.fs", GLSL_VERSION));
        SetShaderValue(shader, GetShaderLocation(shader, "resolution"),
                   (float[2]){ GetScreenWidth(), GetScreenHeight()}, SHADER_UNIFORM_VEC2);
        #endif
        //Initialize Space Ship
        srand((unsigned int)time(NULL));
        spaceShip = (Vector2*)malloc(sizeof(Vector2));
        spaceShip->x = 400.0;
        spaceShip->y = 400.0;
        rotation = 0.0;
        speed = 0.0;

        sprite = LoadTexture("./assets/spaceship.png");

        //Initialize Particles
        for (int i = 0; i < numParticles; i++) {
            particles[i] = (Particle*)malloc(sizeof(Particle));
            particles[i]->velocity = (Vector2){0,0};
            particles[i]->lifetime = 0.0;
            particles[i]->position.x = 10;
            particles[i]->position.y = 10;
            particles[i]->alive = false;
        }
        //Initialize Shoot
        shoot.position.x = 0;
        shoot.position.y = 0;
        shoot.velocity.x = 0;
        shoot.velocity.y = 0;
        shoot.lifetime = 0; 
        shoot.alive = false;

        //Init Asteroids Here
        explosionSpriteSheet = LoadTexture("./assets/explosion.png");//Animation Sprite Sheet
        asteroidSpriteSheet = LoadTexture("./assets/asteroids.png");//Asteroid Variants Sprite Sheet
        for (int i = 0; i < maxAsteroids; i++) {
                    asteroids[i] = (Asteroid*)malloc(sizeof(Asteroid));
                    asteroids[i]->position.x = (float)(rand() % GetScreenWidth());
                    asteroids[i]->position.y = (float)(rand() % GetScreenHeight());
                    asteroids[i]->velocity.x = (float)(rand() % 10 - 5);
                    asteroids[i]->velocity.y = (float)(rand() % 10 - 5);
                    asteroids[i]->rotation = (float)(rand() % 360);
                    asteroids[i]->rotationSpeed = (float)(rand() % 5 - 2.5);
                    if (i < (maxAsteroids / 4)){
                        asteroids[i]->size = (float)(rand() % 30 + 100);
                        asteroids[i]->alive = true;
                    }
                    else{
                        asteroids[i]->alive = false;
                        asteroids[i]->size = (float)(rand() % 30 + 50);
                    }
                    asteroids[i]->asteroidVariant = rand() % 41;
                    asteroids[i]->explosionVariant = rand() % 4;
                    asteroids[i]->currentFrame = 0;
                    asteroids[i]->frameSpeed = 5;
                    asteroids[i]->frameCounter = 0;
                    asteroids[i]->maxFrames = 4;
                    asteroids[i]->fragment = false;
        }
        //Initialize Audio
        InitAudioDevice();
        shootSound =LoadSoundFromWave(LoadWave("./assets/laser.wav"));
        explosionSound =LoadSoundFromWave(LoadWave("./assets/explosion.wav"));
        thrusterSound =LoadSoundFromWave(LoadWave("./assets/thruster.wav"));
}
float x, y;
void gamePlay(int* currScene, int* score){
        //Update Shader Values
        float time = (float)gameFrame*0.01;
        SetShaderValue(shader, GetShaderLocation(shader, "iTime"), &time, SHADER_UNIFORM_FLOAT);
        x = (float)spaceShip->x*0.002;
        SetShaderValue(shader, GetShaderLocation(shader, "iMousex"), &x, SHADER_UNIFORM_FLOAT);
        y = (float)spaceShip->y*0.002;
        SetShaderValue(shader, GetShaderLocation(shader, "iMousey"), &y, SHADER_UNIFORM_FLOAT);
        //-------------------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        //Draw Shader Background
        BeginShaderMode(shader);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK); // Full-screen shader effect
        EndShaderMode() ;

        //Draw Shoot
        if (shoot.alive){
            Vector2 end = {shoot.position.x +shoot.velocity.x, shoot.position.y + shoot.velocity.y};
            DrawLineEx(shoot.position, end, 5.0, WHITE);   
        }
        //Draw Particles
        for (int i = 0; i < numParticles; i++) {
            if (particles[i]->alive)
            {
                Color color = {i % 255,(2*i) % 255,(3*i) % 255,255};
                DrawCircleV(particles[i]->position,5.0, color);
            }
        }
        //Draw Space Ship
        Rectangle source = { 0.0f, 0.0f, sprite.width, sprite.height }; // Full texture
        Rectangle destination = { spaceShip->x, spaceShip->y, 0.25f*sprite.width, 0.25f*sprite.height }; // Destination rectangle
        Vector2 origin = { 0.25f*(sprite.width / 2.0f), 0.25f*(sprite.height / 2.0f)}; // Center of the sprite
        DrawTexturePro(sprite, source, destination,origin, rotation+90.0f, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters


        for (int i = 0; i < maxAsteroids; i++) {
            if (asteroids[i]->alive){

                //Draw Asteroid
                Rectangle source = { (float)(asteroids[i]->asteroidVariant % 7) * (asteroidSpriteSheet.width / 7.0f),
                                     (float)(asteroids[i]->asteroidVariant / 7) * (asteroidSpriteSheet.height / 7.0f),
                                     asteroidSpriteSheet.width / 7.0f,
                                     asteroidSpriteSheet.height / 7.0f }; // Full texture
                Rectangle destination = { asteroids[i]->position.x, asteroids[i]->position.y,
                                          asteroids[i]->size, asteroids[i]->size }; // Destination rectangle
                Vector2 origin = { asteroids[i]->size / 2.0f, asteroids[i]->size / 2.0f }; // Center of the sprite
                DrawTexturePro(asteroidSpriteSheet, source, destination, origin, asteroids[i]->rotation, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters

                //Draw Explosion
                if (asteroids[i]->currentFrame > 0){
                //printf("Current Frame: %d, Explosion Variant: %d\n ", asteroids[i]->currentFrame, asteroids[i]->explosionVariant);
                    if (asteroids[i]->frameCounter++ >= asteroids[i]->frameSpeed){
                        if (asteroids[i]->currentFrame >= asteroids[i]->maxFrames){
                            asteroids[i]->currentFrame = 0;
                        }
                        else{
                            asteroids[i]->currentFrame++;
                        }
                        asteroids[i]->frameCounter = 0;
                        asteroids[i]->alive = false;
                        asteroids[i]->fragment = false;
                    }  
                Rectangle source = { (float)(asteroids[i]->explosionVariant) * (explosionSpriteSheet.width / 5.0f),
                                        ((float)(asteroids[i]->currentFrame-1.0)) * (explosionSpriteSheet.height / 4.0f),
                                        explosionSpriteSheet.width / 5.0f,
                                        (explosionSpriteSheet.height / 4.0f)}; // Full texture
                Rectangle destination = { asteroids[i]->position.x, asteroids[i]->position.y,
                                            asteroids[i]->size, asteroids[i]->size }; // Destination rectangle
                Vector2 origin = { asteroids[i]->size / 2.0f, asteroids[i]->size / 2.0f }; // Center of the sprite
                DrawTexturePro(explosionSpriteSheet, source, destination, origin, asteroids[i]->rotation, WHITE); // Draw a part of a texture defined by a rectangle with 'pro' parameters
                continue;
            }
                
            }
    
        }
        //printf("%d,%d,%d\n",particles[0]->color.r,particles[0]->color.g,particles[0]->color.b);
        //Draw Score
        DrawText(TextFormat("Score: %d", *score), 10, 10, 30, WHITE);
        EndDrawing();
        //-------------------------------------------------------------------------------------------

        update();
        particleHandler();
        asteroidHandler(currScene, score);
        gameFrame++;

        //Spawn an asteroid every 300 frames
        if (gameFrame % 300 == 0){
            int amount = (int)log2(gameFrame/300 + 1);
            int count = 0;
            for (int i = 0; i < maxAsteroids; i++) {
                if (!asteroids[i]->alive){
                    asteroids[i]->position.x = (float)(rand() % GetScreenWidth());
                    asteroids[i]->position.y = (float)(rand() % GetScreenHeight());
                    asteroids[i]->velocity.x = (float)(rand() % 10 - 5 + amount);
                    asteroids[i]->velocity.y = (float)(rand() % 10 - 5 + amount);
                    asteroids[i]->rotation = (float)(rand() % 360);
                    asteroids[i]->rotationSpeed = (float)(rand() % 5 - 2.5);
                    asteroids[i]->size = (float)(rand() % 50 + 120);
                    asteroids[i]->alive = true;
                    asteroids[i]->asteroidVariant = rand() % 41;
                    asteroids[i]->explosionVariant = rand() % 4;
                    asteroids[i]->currentFrame = 0;
                    asteroids[i]->frameSpeed = 5;
                    asteroids[i]->frameCounter = 0;
                    asteroids[i]->maxFrames = 4;
                    asteroids[i]->fragment = false;
                    if (count == amount)
                        break;
                    else
                        count++;
                }
            }
        }
    
}

void update(void){
    //Player Input
    if (IsKeyDown(KEY_RIGHT) && IsKeyDown(KEY_LEFT)){
    }
    else if (IsKeyDown(KEY_RIGHT)){
        rotation += 3.0;
    }
    else if (IsKeyDown(KEY_LEFT)){
        rotation -= 3.0;
    }

    //Update Speed
    if (IsKeyDown(KEY_UP) && speed < 0.15){
        speed += 0.004;
        
    }
    //Thruster Sound
    if (IsKeyDown(KEY_UP) && !IsSoundPlaying(thrusterSound)){
        PlaySound(thrusterSound);
    }
    else if (!IsKeyDown(KEY_UP)){
        StopSound(thrusterSound);
    }

    spaceShip->x += speed*((180/PI)*cosf((PI/180.0)*rotation));
    spaceShip->y += speed*((180/PI)*sinf((PI/180.0)*rotation));

    if (spaceShip->x > GetScreenWidth())
        spaceShip->x = 0;
    else if (spaceShip->x < 0)
        spaceShip->x = GetScreenWidth();

    if (spaceShip->y > GetScreenHeight())
        spaceShip->y = 0;
    else if (spaceShip->y < 0)
        spaceShip->y = GetScreenHeight();
    

    if (speed > 0)
        speed -= 0.001;
}
void asteroidHandler(int* currScene, int* score){
    //Manage Asteroids logic
    for (int i = 0; i < maxAsteroids; i++) {

        if (asteroids[i]->alive){
                //Update Asteroid Position
                
                asteroids[i]->position.x += asteroids[i]->velocity.x * 0.1;
                asteroids[i]->position.y += asteroids[i]->velocity.y * 0.1;
                asteroids[i]->rotation += asteroids[i]->rotationSpeed * 0.1;

                if (asteroids[i]->position.x > GetScreenWidth())
                    asteroids[i]->position.x = 0;
                else if (asteroids[i]->position.x < 0)
                    asteroids[i]->position.x = GetScreenWidth();

                if (asteroids[i]->position.y > GetScreenHeight())
                    asteroids[i]->position.y = 0;
                else if (asteroids[i]->position.y < 0)
                    asteroids[i]->position.y = GetScreenHeight();
                //Check Collision with Shoot and SpaceShip
                if (CheckCollisionCircles(asteroids[i]->position, asteroids[i]->size/2.0,
                                             shoot.position, 5.0) && shoot.alive){
                    PlaySound(explosionSound);
                    ++*score;
                    asteroids[i]->currentFrame = 1;
                    shoot.alive = false;
                    // shoot.lifetime = 0;
                    //Spawn New Asteroid
                    if (asteroids[i]->size > 60.0 && !asteroids[i]->fragment){
                        int count = 1;
                        // int amount = rand() % 4;
                        for (int j = 0; j < maxAsteroids; j++) {
                            if (!asteroids[j]->alive){
                                asteroids[j]->position.x = asteroids[i]->position.x;
                                asteroids[j]->position.y = asteroids[i]->position.y;
                                asteroids[j]->velocity.x = (float)(rand() % 10 - 5);
                                asteroids[j]->velocity.y = (float)(rand() % 10 - 5);
                                asteroids[j]->rotation = (float)(rand() % 360);
                                asteroids[j]->rotationSpeed = (float)(rand() % 5 - 2.5);
                                asteroids[j]->size = (float)(rand() % 30 + 50);
                                asteroids[j]->alive = true;
                                asteroids[j]->asteroidVariant = rand() % 41;
                                asteroids[j]->explosionVariant = rand() % 4;
                                asteroids[j]->currentFrame = 0;
                                asteroids[j]->frameSpeed = 5;
                                asteroids[j]->frameCounter = 0;
                                asteroids[j]->maxFrames = 4;
                                asteroids[j]->fragment = true;
                                if (count == 4)
                                    break;
                                else
                                    count++;
                                }
                            }
                        }   
                    }   
                    if (CheckCollisionCircles(asteroids[i]->position, asteroids[i]->size/2.0,
                                                *spaceShip, 20.0f)){
                        asteroids[i]->currentFrame = 1;
                        PlaySound(explosionSound);
                        if (IsSoundPlaying(thrusterSound)){
                            StopSound(thrusterSound);
                        }
                        *currScene = ENDING;
            
                    }
        }

    }
}
void particleHandler(void)
{
    //Manage Particles logic
    if (IsKeyDown(KEY_SPACE) && shoot.lifetime <= 0){
        const float fast = 40.0;
        float angle_rad = (PI / 180.0f) * rotation;
        shoot.position = *spaceShip;
        shoot.velocity.x = fast* (speed + 1)* cosf(angle_rad);
        shoot.velocity.y = fast * (speed + 1) * sinf(angle_rad);
        shoot.lifetime = 20;
        shoot.alive = true;
        PlaySound(shootSound);
    }

    if (shoot.lifetime > 0){
        shoot.lifetime -= 1;
    }
    if (shoot.alive){
        shoot.position.x += shoot.velocity.x;
        shoot.position.y += shoot.velocity.y;

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
    for (int i = 0; i < maxAsteroids; i++) {
        free(asteroids[i]);
    }
    UnloadShader(shader);
    UnloadTexture(sprite);
    UnloadTexture(explosionSpriteSheet);
    UnloadTexture(asteroidSpriteSheet);
    CloseAudioDevice();
}