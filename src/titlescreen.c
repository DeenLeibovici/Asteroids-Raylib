#include "titlescreen.h"

void titleScreen(int* scene){
    const char* titleText = "Asteroids";
    const char* playText = "Press Enter Key";
    const char* instructionsText = "Use arrow keys to move and spacebar to shoot";
    BeginDrawing();
    // Setup the back buffer for drawing (clear color and depth buffers)
    ClearBackground(BLACK);
    // draw some text using the default font
    int titleOffset = MeasureText(titleText,200);
    int textOffset = MeasureText(playText, 50);
    int instructionsOffset = MeasureText(instructionsText, 50);

    DrawText("Asteroids", (GetScreenWidth()-titleOffset)/2,GetScreenHeight()/2-200,200,WHITE);
    DrawText("Press Enter Key",(GetScreenWidth()-textOffset)/2,GetScreenHeight()/2+100,50,WHITE);
    DrawText(instructionsText,(GetScreenWidth()-instructionsOffset)/2,GetScreenHeight()-200,50,WHITE);

    
    // end the frame and get ready for the next one  (display frame, poll input, etc...)
    EndDrawing();

    if (IsKeyPressed(KEY_ENTER)){
        *scene = GAMEPLAY;
    }
}
