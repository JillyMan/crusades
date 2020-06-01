// crousades.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "crousades_main.h"

#include "core/ecs/ecs.h"
#include "core/particle_system/particles.h"

#include "core/graphics/idraw_context.hpp"
#include "core/graphics/draw_context_factory.hpp"

internal IDrawContext* drawContext = nullptr;

void GameInit(WindowHandle window_handle)
{
	srand(2550);

	graphics_setting Setting;
	Setting.GraphicsApiType = GraphicsApiEnum::DirectX;
	Setting.ResolutionWidth = 1920;
	Setting.ResolutionHeight = 1080;
	Setting.IsWindowed = 1;
	Setting.IsSecondBuffer = 1;
	drawContext = core::graphics::DrawContextFactory::GetDrawContext(Setting, window_handle);

	InitParticleSystem(60000, drawContext);
}

void GameMain(game_input* Input)
{

	drawContext->StartDraw();

	ParticleSystemUpdate(Input->DtPerFrame);

	drawContext->EndDraw();
}

void GameShutdown()
{
	Shutdown();
	delete drawContext;
	//core::graphics::Shutdown();
}
