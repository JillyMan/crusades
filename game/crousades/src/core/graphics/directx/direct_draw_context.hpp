#pragma once

#include <ddraw.h>

#include "core/graphics/ISurface.hpp"
#include "core/graphics/graphics_core.h"
#include "core/graphics/idraw_context.hpp"

namespace core { namespace graphics {

	class DirectDrawContext : public IDrawContext
	{
	public:
		DirectDrawContext(WindowHandle windowHandle, int width, int height);
		DirectDrawContext(WindowHandle windowHandle, int width, int height, int windowed);

		virtual ISurface* CreateDrawSurface(int countBackBuffers = 0) override;

		virtual ~DirectDrawContext();

	private:
		IDirectDraw7* graphicsCardContext = nullptr;
	};

} }
