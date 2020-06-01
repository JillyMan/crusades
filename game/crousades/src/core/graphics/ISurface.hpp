#pragma once

#include "core/graphics/primitives/primitives.h"

namespace core { namespace graphics {

	class ISurface
	{
	public:
		virtual void StartDraw() = 0;
		virtual void EndDraw() = 0;

		virtual void Blit() = 0;
		virtual void Plot32(int x, int y, int color) = 0;
		virtual void DrawBitMap(int x, int y, bit_map& bitMap) = 0;

		virtual void GetVideoMemoryDimension(int& w, int& h) = 0;
	};

} }