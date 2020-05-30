#pragma once

//todo: change this, when game code will moved to dll.

#include "core/graphics/graphics_core.h"

void GameInit(WindowHandle window);
void GameMain(game_input* Input);
void GameShutdown();
