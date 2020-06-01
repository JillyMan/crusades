#pragma once

#include "../graphics_core.h"

namespace core { namespace graphics {

	void Init(WindowHandle windowHandle, graphics_setting graphicsSetting);

	void StartDraw();
	void EndDraw();

	void GetVideoMemoryDimension(int& w, int& h);

	void Plot32(int x, int y, int color);

	void Shutdown();
}}