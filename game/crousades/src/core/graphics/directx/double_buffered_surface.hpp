#pragma once

#include <ddraw.h>

#include "base_draw_surface.hpp"

namespace core { namespace graphics {

	class DoubleBufferedSurface : public BaseDrawSurface
	{
	public:
		DoubleBufferedSurface(IDirectDraw7* graphicsCardContext);
		~DoubleBufferedSurface();

		virtual void EndDraw() override;

	private:
		void InitPrimarySurface(IDirectDraw7* graphicsCardContext);
		void InitSecondarySurface();

	private:
		IDirectDrawSurface7* primarySurface = nullptr;
	};

} }