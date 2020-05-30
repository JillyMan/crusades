#pragma once

#include "core/ecs/ecs.h"
#include <windows.h>

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

void InitParticleSystem(int particleCount);
void ParticleSystemUpdate(float dt);
void Shutdown();
