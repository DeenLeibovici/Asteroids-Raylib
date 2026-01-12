#include "raylib.h"
#include "titlescreen.h"
#include "gameplay.h"
#include "gameover.h"
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

int* score;
int* currScene;
#if defined(PLATFORM_WEB)
bool one_iter(double time, void* userData) {
switch(*currScene){
			case TITLESCREEN:
			{
				titleScreen(currScene);
				break;
			}
			case GAMEPLAY:
			{
				gamePlay(currScene, score);
				break;
			}
			case ENDING:
			{
				gameOver(currScene, score);
				break;
			}
		}

	return true;
}
#endif

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1920, 1080, "Asteroids");
	SetTargetFPS(60);
	currScene = malloc(sizeof(int));
	*currScene = TITLESCREEN;
	

	score = (int*)malloc(sizeof(int));
	*score = 0;
	initGamePlay();
	// game loop
	#if defined(PLATFORM_DESKTOP)
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{

		switch(*currScene){
			case TITLESCREEN:
			{
				titleScreen(currScene);
				break;
			}
			case GAMEPLAY:
			{
				gamePlay(currScene, score);
				break;
			}
			case ENDING:
			{
				gameOver(currScene, score);
				break;
			}
		}
		
	}
	#else
	  emscripten_request_animation_frame_loop(one_iter, NULL);
	  return 0;
	#endif
	if (*currScene == GAMEPLAY)
		cleanup();
	free(currScene);
	free(score);
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
