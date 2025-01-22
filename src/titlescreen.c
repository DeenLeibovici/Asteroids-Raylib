#include "titlescreen.h"

void titleScreen(Scene* scene){
    const char* titleText = "Asteroids";
    const char* playText = "Press Enter Key";
    BeginDrawing();
    // Setup the back buffer for drawing (clear color and depth buffers)
    ClearBackground(BLACK);
    // draw some text using the default font
    int titleOffset = MeasureText(titleText,100);
    int textOffset = MeasureText(playText, 40);
    DrawText("Asteroids", (1280-titleOffset)/2,300,100,WHITE);
    DrawText("Press Enter Key",(1280-textOffset)/2,450,40,WHITE);
    
    // end the frame and get ready for the next one  (display frame, poll input, etc...)
    EndDrawing();

    if (IsKeyPressed(KEY_ENTER)){
        *scene = GAMEPLAY;
    }
}
