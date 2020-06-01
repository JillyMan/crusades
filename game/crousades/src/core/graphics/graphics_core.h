#pragma once

#ifdef _WIN32
	#include <Windows.h>
	typedef HWND WindowHandle;
#elif
	typedef void* WindowHandle;
#endif

#define RED_COLOR RGB(0, 0, 255)
#define WHITE_COLOR RGB(255, 255, 255)
