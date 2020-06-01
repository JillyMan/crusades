#pragma once

#include <windows.h>

#include "core/graphics/ISurface.hpp"

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

void InitParticleSystem(int particleCount, core::graphics::ISurface* drawContext);
void ParticleSystemUpdate(float dt);
void Shutdown();
