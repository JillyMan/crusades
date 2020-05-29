#include <pch.h>
#include <ddraw.h>

#include "graphics_core.h"
#include "graphics.h"

internal LPDIRECTDRAW DirectDraw = nullptr;

namespace core { namespace graphics {

	void Init(int width, int height)
	{
		HRESULT result = DirectDrawCreate(nullptr, &DirectDraw, nullptr);

		assert(SUCCEEDED(result));
		DirectDraw->SetDisplayMode(width, height, 256);
	}

	void Shutdown()
	{
		RELEASE_COM(DirectDraw);
	}

}}