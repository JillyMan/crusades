#pragma once

#include <ddraw.h>

#include "core/graphics/graphics_core.h"
#include "core/graphics/idraw_context.hpp"
#include "core/graphics/directx/draw_surface.hpp"

namespace core { namespace graphics {

	class DirectDrawContext : public IDrawContext
	{
	public:
		DirectDrawContext(WindowHandle windowHandle, int width, int height);
		DirectDrawContext(WindowHandle windowHandle, int width, int height, int windowed);
		DirectDrawContext(WindowHandle windowHandle, int width, int height, int windowed, int secondBuffer);

		virtual ~DirectDrawContext();

		virtual void StartDraw() override;
		
		inline virtual void Plot32(int x, int y, int color) override
		{
			unsigned int* memory = (unsigned int*) backBufferInfo.Memory;
			memory[x + y * backBufferInfo.Pitch32] = color;
		}

		inline virtual void GetVideoMemoryDimension(int& w, int& h) override 
		{
			directDrawSurface->GetVideoMemoryDimension(w, h);
		}

		virtual void EndDraw() override;

	private:
		IDirectDraw7* graphicsCardContext = nullptr;
		DrawSurface* directDrawSurface = nullptr;

		back_buffer_info backBufferInfo;
	};

} }
