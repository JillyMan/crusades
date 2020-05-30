// crousades.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "crousades_main.h"

#include "core/ecs/ecs.h"
#include "core/graphics/graphics.h"
#include "core/particle_system/particles.h"

void GameInit(WindowHandle window_handle)
{
	srand(2550);
	core::graphics::Init(window_handle, 1920, 1080);

	InitParticleSystem(10000);
}

void GameMain(game_input* Input)
{
	ParticleSystemUpdate(Input->DtPerFrame);
}

void GameShutdown()
{
	Shutdown();
}
