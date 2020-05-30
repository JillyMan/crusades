#pragma once

#ifdef _WIN32
	#include <Windows.h>
	#define RELEASE_COM(x) if (x) { x->Release(); x = nullptr; }
	typedef HWND WindowHandle;
#elif
	typedef void* WindowHandle;
#endif
