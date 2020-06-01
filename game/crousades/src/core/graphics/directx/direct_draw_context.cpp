#include "pch.h"

#include "direct_draw_context.hpp"

#include "core/graphics/directx/common.h"
#include "core/graphics/directx/single_buffered_surface.hpp"
#include "core/graphics/directx/double_buffered_surface.hpp"

namespace core { namespace graphics {

	DirectDrawContext::DirectDrawContext(WindowHandle windowHandle, int width, int height)
		: DirectDrawContext(windowHandle, width, height, 1)
	{
	}

	DirectDrawContext::DirectDrawContext(WindowHandle windowHandle, int width, int height, int windowed)
		: graphicsCardContext(nullptr)
	{
		HRESULT result = DirectDrawCreateEx(NULL, (void**)&(graphicsCardContext), IID_IDirectDraw7, NULL);
		assert(SUCCEEDED(result));

		if (windowed)
		{
			result = graphicsCardContext->SetCooperativeLevel(windowHandle, DDSCL_NORMAL);
			assert(SUCCEEDED(result));
		}
		else {
			result = graphicsCardContext->SetCooperativeLevel(windowHandle,
				DDSCL_FULLSCREEN	|
				DDSCL_EXCLUSIVE		|
				DDSCL_ALLOWREBOOT	|
				DDSCL_MULTITHREADED
			);

			result = graphicsCardContext->SetDisplayMode(width, height, DEFAULT_BPP_MODE_BIT, 0, 0);
			assert(SUCCEEDED(result));
		}
	}

	DirectDrawContext::~DirectDrawContext() 
	{
		RELEASE_COM(this->graphicsCardContext);
	}

	ISurface* DirectDrawContext::CreateDrawSurface(int countBackBuffers)
	{
		if (countBackBuffers == 0) 
		{
			return new SingleBufferedSurface(this->graphicsCardContext);
		}

		return new DoubleBufferedSurface(this->graphicsCardContext);
	}

}}
