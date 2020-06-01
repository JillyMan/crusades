#pragma once

namespace core { namespace graphics {

	struct back_buffer_info
	{
		int Width;
		int Height;
		int Pitch32;	// Pitch32 because pixel consists of 32bit
		int MemorySize;
		void* Memory;
	};

	class DrawSurface
	{
	public:
		DrawSurface(IDirectDraw7* direct_draw, bool secondBuffer);
		~DrawSurface();

		void Lock(back_buffer_info& info);

		void GetVideoMemoryDimension(int& w, int& h);

		void Flip();

	private:
		DDSURFACEDESC2 GetDesc(bool backbuffer);

	private:
		IDirectDrawSurface7* primarySurface = nullptr;
		IDirectDrawSurface7* secondSurface = nullptr;

		IDirectDrawSurface7* workingSurface = nullptr;
	};

}}
