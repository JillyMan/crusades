#include "pch.h"

#include <ddraw.h>

#include "core/graphics/directx/common.h"
#include "core/graphics/directx/draw_surface.hpp"

namespace core { namespace graphics {

	DrawSurface::DrawSurface(IDirectDraw7* direct_draw, bool secondBuffer)
	{
		DDSURFACEDESC2 surfaceDescription = GetDesc(secondBuffer);

		HRESULT result = direct_draw->CreateSurface(&surfaceDescription, &primarySurface, NULL);
		assert(SUCCEEDED(result));
		workingSurface = primarySurface;

		if (secondBuffer)
		{
			DDSCAPS2 ddCaps = {};
			ddCaps.dwCaps = DDSCAPS_BACKBUFFER;

			result = (primarySurface)->GetAttachedSurface(&ddCaps, &secondSurface);
			assert(SUCCEEDED(result));

			workingSurface = secondSurface;
		}
	}

	DrawSurface::~DrawSurface()
	{
		RELEASE_COM(this->secondSurface);
		RELEASE_COM(this->primarySurface);
	}

	DDSURFACEDESC2 DrawSurface::GetDesc(bool backbuffer)
	{
		DDSURFACEDESC2 surfaceDescription = { };
		DDRAW_INIT_STRUCT(surfaceDescription)

		surfaceDescription.dwFlags = DDSD_CAPS;
		surfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		if (backbuffer)
		{
			surfaceDescription.dwBackBufferCount = 1;
			surfaceDescription.dwFlags |= DDSD_BACKBUFFERCOUNT;
			surfaceDescription.ddsCaps.dwCaps |= DDSCAPS_COMPLEX | DDSCAPS_FLIP;
		}

		return surfaceDescription;
	}

	void DrawSurface::Lock(back_buffer_info& info)
	{
		DDSURFACEDESC2 surfaceDesc = { };
		DDRAW_INIT_STRUCT(surfaceDesc)

		HRESULT result = workingSurface->Lock(NULL, &surfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
		if (result == DDERR_SURFACELOST)
		{
			result = workingSurface->Restore();
		}
		assert(SUCCEEDED(result));

		info.Pitch32 = (surfaceDesc.lPitch >> 2); // shift >> 2 - because color depth 32bit
		info.Width = surfaceDesc.dwWidth;
		info.Height = surfaceDesc.dwHeight;
		info.MemorySize = surfaceDesc.lPitch * surfaceDesc.dwHeight;
		info.Memory = (uint32_t*)surfaceDesc.lpSurface;

		memset(info.Memory, 0, info.MemorySize);
	}

	void DrawSurface::GetVideoMemoryDimension(int& w, int& h)
	{
		DDSURFACEDESC2 surfaceDesc = { };
		DDRAW_INIT_STRUCT(surfaceDesc)

		HRESULT result = primarySurface->GetSurfaceDesc(&surfaceDesc);
		assert(SUCCEEDED(result));

		w = surfaceDesc.dwWidth;
		h = surfaceDesc.dwHeight;
	}

	void DrawSurface::Flip()
	{
		HRESULT result = workingSurface->Unlock(NULL);
		assert(SUCCEEDED(result));

		if (secondSurface != nullptr) {

			while (FAILED(primarySurface->Flip(NULL, DDFLIP_WAIT)));
		}
	}

} }
