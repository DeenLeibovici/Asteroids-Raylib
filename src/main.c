#include "raylib.h"
#include "titlescreen.h"
#include "gameplay.h"
#include <stdlib.h>


#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

// void titleScreen();
// #include "resource_dir.h"	// utility header for SearchAndSetResourceDir

// typedef enum Scene {TITLESCREEN = 0, GAMEPLAY, ENDING} Scene;
int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Asteroids");
	SetTargetFPS(60);
	Scene* currScene = (Scene*)malloc(sizeof(Scene));
	*currScene = TITLESCREEN;
	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	// SearchAndSetResourceDir("resources");


	// game loop
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
				gamePlay();
				break;
			}
			case ENDING:
			{
				//ending();
				break;
			}
		}
		
	}

	// cleanup
	if (currScene  == GAMEPLAY){
		cleanup();
	}
	free(currScene);
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
