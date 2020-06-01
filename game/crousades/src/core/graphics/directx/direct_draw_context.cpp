#include "pch.h"

#include "core/graphics/directx/common.h"
#include "direct_draw_context.hpp"

namespace core { namespace graphics {

	DirectDrawContext::DirectDrawContext(WindowHandle windowHandle, int width, int height)
		: DirectDrawContext(windowHandle, width, height, 1)
	{
	}

	DirectDrawContext::DirectDrawContext(WindowHandle windowHandle, int width, int height, int windowed) 
		: DirectDrawContext(windowHandle, width, height, windowed, 0)
	{
	}

	DirectDrawContext::DirectDrawContext(WindowHandle windowHandle, int width, int height, int windowed, int secondBackBuffer)
		: backBufferInfo(),
		graphicsCardContext(nullptr)
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

		directDrawSurface = new DrawSurface(
			graphicsCardContext, 
			!windowed && secondBackBuffer
		);
	}

	DirectDrawContext::~DirectDrawContext() 
	{
		delete directDrawSurface;
		RELEASE_COM(this->graphicsCardContext);
	}

	void DirectDrawContext::StartDraw()
	{
		directDrawSurface->Lock(backBufferInfo);
	}

	void DirectDrawContext::EndDraw() 
	{
		directDrawSurface->Flip();

		//todo: check - what will happen if write something in backBufferInfo.Memory, after Flip ??
		backBufferInfo = { };
	}

}}
