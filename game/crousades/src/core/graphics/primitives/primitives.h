#pragma once

namespace core { namespace graphics {

	struct rectangle {
		int x, y;
		int w, h;
	};

	struct bit_map
	{
		int Width;
		int Height;
		void* Memory;

		bit_map(int width, int height, void* memory)
			: Width(width),
			Height(height),
			Memory(memory)
		{
		}
	};

} }
