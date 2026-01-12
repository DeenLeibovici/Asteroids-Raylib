#include "gameover.h"

void gameOver(int* currScene, int* score){
    ClearBackground(BLACK);
    const char* gameOverText = "Game Over!";
    const char* restartText = "Press Enter to Restart";
    const char* scoreTextFormat = "Final Score: %d";
    int gameOverTextOffset = MeasureText(gameOverText, 200);
    int restartTextOffset = MeasureText(restartText, 50);
    int scoreTextOffset = MeasureText(TextFormat(scoreTextFormat, *score), 50);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(gameOverText, GetScreenWidth() / 2 - gameOverTextOffset / 2, GetScreenHeight()/2 - 200, 200, RED);
    DrawText(TextFormat(scoreTextFormat, *score), GetScreenWidth() / 2 - scoreTextOffset / 2, GetScreenHeight()/2 + 20, 50, WHITE);
    DrawText(restartText, GetScreenWidth() / 2 - restartTextOffset / 2, GetScreenHeight() - 100, 50, WHITE);
    EndDrawing();

    if (IsKeyDown(KEY_ENTER)){
        cleanup();
        initGamePlay();
        *score = 0;
        *currScene = TITLESCREEN;
    }
}