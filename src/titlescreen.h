#pragma once
#include "raylib.h"

// Define the different scenes in the game(This method is used instead of enum for web compatibility)
#define TITLESCREEN 0
#define GAMEPLAY 1
#define ENDING 2
void titleScreen(int* scene);

