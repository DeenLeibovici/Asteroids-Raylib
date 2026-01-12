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

// void titleScreen();
// #include "resource_dir.h"	// utility header for SearchAndSetResourceDir
//Command I used to compile for web: emcc -o asteroid_c.html src/main.c src/titlescreen.c src/gameplay.c src/gameover.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os -I. -I C:/libs/raylib/raylib/src -I C:/libs/raylib/raylib/src/external -L. -L C:/libs/raylib/raylib/src -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --shell-file C:/libs/raylib/raylib/src/shell.html C:/libs/raylib/raylib/src/web/libraylib.a -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]'-s EXPORTED_RUNTIME_METHODS=ccall --preload-file assets -s ASYNCIFY
//Global variable used for web compatibility

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
	#if defined(PLATFORM_WEB)
	*currScene = GAMEPLAY;
	#else
	*currScene = TITLESCREEN;
	#endif

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
	// cleanup
	if (*currScene  == GAMEPLAY){
		cleanup();
	}
	free(currScene);
	free(score);
	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
