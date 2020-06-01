#include "pch.h"

#include "core/graphics/directx/common.h"
#include "core/graphics/directx/direct_draw_context.hpp"

#include "double_buffered_surface.hpp"

namespace core { namespace graphics {

	DoubleBufferedSurface::DoubleBufferedSurface(IDirectDraw7* graphicsCardContext) 
	{
		InitPrimarySurface(graphicsCardContext);
		InitSecondarySurface();
	}

	void DoubleBufferedSurface::InitPrimarySurface(IDirectDraw7* graphicsCardContext)
	{
		DDSURFACEDESC2 surfaceDescription = { };
		DDRAW_INIT_STRUCT(surfaceDescription)
		surfaceDescription.dwBackBufferCount = 1;

		surfaceDescription.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		surfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

		HRESULT result = graphicsCardContext->CreateSurface(&surfaceDescription, &primarySurface, NULL);
		assert(SUCCEEDED(result));
	}

	void DoubleBufferedSurface::InitSecondarySurface()
	{
		DDSCAPS2 ddCaps = {};
		ddCaps.dwCaps = DDSCAPS_BACKBUFFER;

		IDirectDrawSurface7 *secondarySurface = nullptr;
		HRESULT result = primarySurface->GetAttachedSurface(&ddCaps, &secondarySurface);
		assert(SUCCEEDED(result));

		workingSurface = secondarySurface;
	}

	DoubleBufferedSurface::~DoubleBufferedSurface()
	{
		RELEASE_COM(workingSurface)
		RELEASE_COM(primarySurface)
	}

	void DoubleBufferedSurface::EndDraw()
	{
		workingSurface->Unlock(NULL);

		while (FAILED(primarySurface->Flip(NULL, DDFLIP_WAIT)));
	}

} }
