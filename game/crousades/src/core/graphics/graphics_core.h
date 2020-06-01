#pragma once

#ifdef _WIN32
	#include <Windows.h>
	typedef HWND WindowHandle;
#elif
	typedef void* WindowHandle;
#endif

#define WHITE_COLOR RGB(255, 255, 255)
