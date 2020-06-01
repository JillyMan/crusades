#pragma once

#include "ISurface.hpp"

#include "core/graphics/primitives/primitives.h"

namespace core { namespace graphics {

	class IDrawContext
	{
	public:
		virtual ISurface* CreateDrawSurface(int countBackBuffers = 0) = 0;
	};
}}