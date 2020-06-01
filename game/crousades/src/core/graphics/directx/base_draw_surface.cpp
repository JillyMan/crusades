#include "pch.h"

#include "base_draw_surface.hpp"

namespace core { namespace graphics {

	BaseDrawSurface::BaseDrawSurface()
		: backBufferInfo()
	{
	}

	BaseDrawSurface::~BaseDrawSurface()
	{
	}

	void BaseDrawSurface::StartDraw()
	{
		DDSURFACEDESC2 surfaceDesc = { };
		DDRAW_INIT_STRUCT(surfaceDesc)

		HRESULT result = workingSurface->Lock(NULL, &surfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
		if (result == DDERR_SURFACELOST)
		{
			result = workingSurface->Restore();
		}
		assert(SUCCEEDED(result));

		backBufferInfo.Pitch32 = (surfaceDesc.lPitch >> 2); // shift >> 2 - because color depth 32bit
		backBufferInfo.Width = surfaceDesc.dwWidth;
		backBufferInfo.Height = surfaceDesc.dwHeight;
		backBufferInfo.MemorySize = surfaceDesc.lPitch * surfaceDesc.dwHeight;
		backBufferInfo.Memory = (uint32_t*)surfaceDesc.lpSurface;

		//memset(backBufferInfo.Memory, 0, backBufferInfo.MemorySize);
	}

	// Warning: this function will has bug if screen is in windowed mode.
	void BaseDrawSurface::Clear(int color)
	{
		DDBLTFX bltSetting;
		DDRAW_INIT_STRUCT(bltSetting);
		
		bltSetting.dwFillColor = color;

		int w, h;
		GetVideoMemoryDimension(w, h);

		RECT descRect;
		descRect.left = 0;
		descRect.top = 0;
		descRect.right = w;
		descRect.bottom = h;

		HRESULT result = workingSurface->Blt(&descRect, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &bltSetting);
		assert(SUCCEEDED(result));
	}

	void BaseDrawSurface::DrawBitMap(int x, int y, bit_map& bitMap)
	{
		assert(backBufferInfo.Memory);

		uint32_t* videoMemory = (uint32_t*)backBufferInfo.Memory + (x + y * backBufferInfo.Pitch32);

		uint32_t* bitMapMemory = (uint32_t*)bitMap.Memory;

		for (int row = 0; row < bitMap.Width; ++row)
		{
			uint32_t* bitmapStart = bitMapMemory;
			uint32_t* startVideoMemory = videoMemory;

			for (int col = 0; col < bitMap.Height; ++col)
			{
				int color = *bitmapStart++;
				if (color != WHITE_COLOR)
				{
					*(startVideoMemory++) = color;
				}
			}
			bitMapMemory += bitMap.Width;
			videoMemory += backBufferInfo.Pitch32;
		}
	}

	void BaseDrawSurface::GetVideoMemoryDimension(int& w, int& h)
	{
		DDSURFACEDESC2 surfaceDesc = { };
		DDRAW_INIT_STRUCT(surfaceDesc);

		HRESULT result = workingSurface->GetSurfaceDesc(&surfaceDesc);
		assert(SUCCEEDED(result));

		w = surfaceDesc.dwWidth;
		h = surfaceDesc.dwHeight;
	}

}}