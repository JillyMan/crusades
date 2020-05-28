#include "pch.h"

#include "core/particle_system/particles.h"

internal HDC InternalWindowDC;
internal HWND GlobalWindowHandle;
internal particle* ParticleBuffer;
internal int ParticleCount;

internal void GetWindowDimension(int &w, int &h)
{
	RECT rect = {};
	GetClientRect(GlobalWindowHandle, &rect);

	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
}

void InitParticleSystem(HWND window_handle, int particleCount)
{
	GlobalWindowHandle = window_handle;
	InternalWindowDC = GetDC(GlobalWindowHandle);

	ParticleCount = particleCount;
	ParticleBuffer = (particle*)malloc(sizeof(particle) * ParticleCount);

	int w, h;
	GetWindowDimension(w, h);

	for (int i = 0; i < ParticleCount; ++i)
	{
		particle p;
		p.x = (float)(rand() % w);
		p.y = (float)(rand() % h);
		p.ax = (float)100 + rand() % 200;
		p.ay = 0;
		p.color = (255 << 16 | 255 << 8 | 255);

		ParticleBuffer[i] = p;
	}
}

internal void EraseScreen(particle* particleBuffer, int count)
{
	for (int i = 0; i < ParticleCount; ++i)
	{
		particle& _particle = ParticleBuffer[i];
		SetPixel(InternalWindowDC, _particle.x, _particle.y, RGB( 0, 0, 0 ));
	}
}

internal void Update(particle* particleBuffer, int count, float dt)
{
	int w, h;
	GetWindowDimension(w, h);

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
		SetPixel(InternalWindowDC, _particle.x, _particle.y, RGB(
			_particle.r,
			_particle.g,
			_particle.b
		));
	}
}

void ParticleSystemUpdate(float dt)
{
	EraseScreen(ParticleBuffer, ParticleCount);
	Update(ParticleBuffer, ParticleCount, dt);
	Render(ParticleBuffer, ParticleCount);
}

void Shutdown()
{
	ReleaseDC(GlobalWindowHandle, InternalWindowDC);
}