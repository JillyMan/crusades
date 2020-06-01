#pragma once
#include <ddraw.h>

#include "core/graphics/ISurface.hpp"
#include "core/graphics/directx/base_draw_surface.hpp"

namespace core { namespace graphics {

	class SingleBufferedSurface : public BaseDrawSurface
	{
	public:
		SingleBufferedSurface(IDirectDraw7* graphicsCardContext);
		~SingleBufferedSurface();
		virtual void EndDraw() override;

	private:
		IDirectDrawSurface7* primarySurface = nullptr;
	};

} }