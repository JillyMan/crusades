// crousades.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "crousades_main.h"

#include "core/ecs/ecs.h"
#include "core/particle_system/particles.h"

#include "core/graphics/idraw_context.hpp"
#include "core/graphics/draw_context_factory.hpp"
#include "core/graphics/ISurface.hpp"

internal core::graphics::IDrawContext* drawContext = nullptr;
internal core::graphics::ISurface* surface = nullptr;
internal core::graphics::bit_map* PlayerBitMap;
internal float playerX = 10, playerY = 100, speedX = 128;

#define PLAYER_SPRITE_SIZE_W 900
int playerBitMapBuffer[PLAYER_SPRITE_SIZE_W * PLAYER_SPRITE_SIZE_W];

void GameInit(WindowHandle window_handle)
{
	srand(123);

	graphics_setting Setting;
	Setting.GraphicsApiType = GraphicsApiEnum::DirectX;
	Setting.ResolutionWidth = 1920;
	Setting.ResolutionHeight = 1080;
	Setting.IsWindowed = 1;
	Setting.SecondBufferCount = Setting.IsWindowed ? 0 : 1;

	drawContext = core::graphics::DrawContextFactory::GetDrawContext(Setting, window_handle);
	surface = drawContext->CreateDrawSurface(Setting.SecondBufferCount);

	for (int i = 0; i < PLAYER_SPRITE_SIZE_W; ++i)
	{
		for (int j = 0; j < PLAYER_SPRITE_SIZE_W; ++j)
		{
			if (j < i)
			{
				playerBitMapBuffer[i + j * PLAYER_SPRITE_SIZE_W] = WHITE_COLOR;
			}
			else {
				playerBitMapBuffer[i + j * PLAYER_SPRITE_SIZE_W] = RED_COLOR;
			}
		}
	}

	PlayerBitMap = new core::graphics::bit_map(
		PLAYER_SPRITE_SIZE_W, PLAYER_SPRITE_SIZE_W, (void*)playerBitMapBuffer
	);

	InitParticleSystem(60000, surface);
}

void GameMain(game_input* Input)
{

	surface->StartDraw();
	
	playerX += speedX * Input->DtPerFrame;

	if (playerX + PLAYER_SPRITE_SIZE_W > 1920 || playerX < 0)
	{
		speedX = -speedX;
		playerX += speedX * Input->DtPerFrame;
	}

	surface->DrawBitMap(playerX, playerY, *PlayerBitMap);

	//ParticleSystemUpdate(Input->DtPerFrame);

	surface->EndDraw();
}

void GameShutdown()
{
	Shutdown();
	delete drawContext;
	//core::graphics::Shutdown();
}
