#pragma once

#include "graphics_core.h"

namespace core { namespace graphics {

	void Init(WindowHandle windowHandle, int width, int height);

	void LockVideoMemory();
	void UnlockVideoMemory();

	void GetVideoMemoryDimension(int& w, int& h);

	void Plot32(int x, int y, int color);

	void Shutdown();
}}