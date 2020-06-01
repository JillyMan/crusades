#pragma once

#include <ddraw.h>

#include "common.h"
#include "core/graphics/ISurface.hpp"

namespace core { namespace graphics {

	struct back_buffer_info
	{
		int Width;
		int Height;
		int Pitch32;	// Pitch32 because pixel consists of 32bit
		int MemorySize;
		void* Memory;
	};

	class BaseDrawSurface : public ISurface
	{
	public:
		BaseDrawSurface();
		~BaseDrawSurface();

		virtual void StartDraw() override;

		virtual void Blit() override;
		virtual void DrawBitMap(int x, int y, bit_map& bitMap) override;
		virtual void GetVideoMemoryDimension(int& w, int& h) override;

		inline virtual void Plot32(int x, int y, int color) override
		{
			unsigned int* memory = (unsigned int*)backBufferInfo.Memory;
			memory[x + y * backBufferInfo.Pitch32] = color;
		}

	protected:
		IDirectDrawSurface7* workingSurface = nullptr;

	private:
		back_buffer_info backBufferInfo;
	};
} }