#pragma once

//todo: change this, when game code will moved to dll.

#include <Windows.h>

void GameInit(HWND window);
void GameMain(game_input* Input);
void GameShutdown();
