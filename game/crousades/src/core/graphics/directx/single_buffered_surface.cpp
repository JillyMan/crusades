#include "pch.h"

#include "core/graphics/directx/common.h"
#include "single_buffered_surface.hpp"

namespace core { namespace graphics {

	SingleBufferedSurface::SingleBufferedSurface(IDirectDraw7* graphicsCardContext)
	{
		DDSURFACEDESC2 surfaceDescription = { };
		DDRAW_INIT_STRUCT(surfaceDescription)

		surfaceDescription.dwFlags = DDSD_CAPS;
		surfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		HRESULT result = graphicsCardContext->CreateSurface(&surfaceDescription, &primarySurface, NULL);
		assert(SUCCEEDED(result));

		workingSurface = primarySurface;
	}

	SingleBufferedSurface::~SingleBufferedSurface()
	{
		RELEASE_COM(primarySurface);
	}

	void SingleBufferedSurface::EndDraw()
	{
		primarySurface->Unlock(NULL);
	}

} }
