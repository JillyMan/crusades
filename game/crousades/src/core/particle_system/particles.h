#pragma once

#include <windows.h>

#include "core/graphics/idraw_context.hpp"

struct particle
{
	float x, y;
	// acceleration
	float ax, ay;
	
	union {
		int color;
		struct {
			unsigned char r, g, b, a;
		};
	};
};

void InitParticleSystem(int particleCount, IDrawContext* drawContext);
void ParticleSystemUpdate(float dt);
void Shutdown();
