// crousades.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "crousades_main.h"

#include "core/ecs/ecs.h"
#include "core/particle_system/particles.h"
#include "core/graphics/graphics.h"

void GameInit(HWND window_handle)
{
	srand(2550);
	InitParticleSystem(window_handle, 2000);
	core::graphics::Init(1280, 720);
}

void GameMain(game_input* Input)
{
	ParticleSystemUpdate(Input->DtPerFrame);
}

void GameShutdown()
{
	Shutdown();
}
