#include "pch.h"

#include "core/particle_system/particles.h"

internal particle* ParticleBuffer;
internal int ParticleCount;

internal core::graphics::ISurface* surface;

void InitParticleSystem(int particleCount, core::graphics::ISurface* _surface)
{
	ParticleCount = particleCount;
	ParticleBuffer = (particle*)malloc(sizeof(particle) * ParticleCount);

	surface = _surface;

	int w, h;
	surface->GetVideoMemoryDimension(w, h);

	for (int i = 0; i < ParticleCount; ++i)
	{
		particle p;
		p.x = (float)(rand() % (w - 1));
		p.y = (float)(rand() % (h - 1));
		p.ax = (float)100 + rand() % 400;
		p.ay = 0;
		p.color = (255 << 16 | 255 << 8 | 255);

		ParticleBuffer[i] = p;
	}
}

internal void Update(particle* particleBuffer, int count, float dt)
{
	int w, h;
	surface->GetVideoMemoryDimension(w, h);
	
	for (int i = 0; i < ParticleCount; ++i)
	{
		particle& _particle = ParticleBuffer[i];
		_particle.x += _particle.ax * dt;
		_particle.y += _particle.ay * dt;

		if (_particle.x > w)
		{
			_particle.x = 0;
		}
	}
}

internal void Render(particle* particleBuffer, int count)
{
	for (int i = 0; i < ParticleCount; ++i)
	{
		particle& _particle = ParticleBuffer[i];
		
		surface->Plot32(_particle.x, _particle.y, RGB(
			_particle.r,
			_particle.g,
			_particle.b
		));
	}
}

void ParticleSystemUpdate(float dt)
{
	Update(ParticleBuffer, ParticleCount, dt);
	Render(ParticleBuffer, ParticleCount);
}

void Shutdown()
{
	free(ParticleBuffer);
	ParticleBuffer = nullptr;
	ParticleCount = 0;
}