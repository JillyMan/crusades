#pragma once

#include "graphics_core.h"

namespace core { namespace graphics {

#define WHITE_COLOR RGB(255, 255, 255)

	void Init(WindowHandle windowHandle, int width, int height, int windowed);

	void StartDraw();
	void EndDraw();

	void GetVideoMemoryDimension(int& w, int& h);

	void Plot32(int x, int y, int color);

	void ClearBuffer32(int color = WHITE_COLOR);

	void Shutdown();
}}