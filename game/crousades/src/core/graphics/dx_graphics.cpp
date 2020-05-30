#include <pch.h>
#include <ddraw.h>

#include "graphics_core.h"
#include "graphics.h"

struct back_buffer
{
	int Width;
	int Height;
	long Pitch;
	void *VideoMemory;
};

internal IDirectDraw7 *GraphicsCardContext = nullptr;
internal IDirectDrawPalette *GraphicsPalette = nullptr;
internal IDirectDrawSurface7 *GraphicsSurface = nullptr;

internal back_buffer VideoBuffer;
internal PALETTEENTRY Palette[256];

namespace core { namespace graphics {

	void InitDirectDraw(IDirectDraw7** direct_draw, WindowHandle window_handle, int width, int height)
	{
		HRESULT result = DirectDrawCreateEx(NULL, (void**)&(*direct_draw), IID_IDirectDraw7, NULL);
		assert(SUCCEEDED(result));

		result = (*direct_draw)->SetCooperativeLevel(window_handle,
			//DDSCL_FULLSCREEN |
			//DDSCL_ALLOWMODEX |
			//DDSCL_EXCLUSIVE |
			//DDSCL_ALLOWREBOOT
			DDSCL_NORMAL
		);
	
		assert(SUCCEEDED(result));

		result = (*direct_draw)->SetDisplayMode(width, height, 32, 0, 0);
		assert(SUCCEEDED(result));
	}

	internal void InitPalete(IDirectDraw7* direct_draw, IDirectDrawPalette** direct_palette)
	{
		for (int color = 1; color < 255; color++)
		{
			// Заполнение случайными значениями RGB
			Palette[color].peRed = rand() % 256;
			Palette[color].peGreen = rand() % 256;
			Palette[color].peBlue = rand() % 256;
			// Установка флага PC_NOCOLLAPSE
			Palette[color].peFlags = PC_NOCOLLAPSE;
		}

		Palette[0].peRed = 0;
		Palette[0].peGreen = 0;
		Palette[0].peBlue = 0;
		Palette[0].peFlags = PC_NOCOLLAPSE;

		Palette[255].peRed = 255;
		Palette[255].peGreen = 255;
		Palette[255].peBlue = 255;
		Palette[255].peFlags = PC_NOCOLLAPSE;

		HRESULT result = direct_draw->CreatePalette(
			DDPCAPS_8BIT | DDPCAPS_ALLOW256 | DDPCAPS_INITIALIZE,
			Palette, direct_palette, NULL
		);

		assert(SUCCEEDED(result));
	}

	internal void InitSurface(IDirectDrawSurface7** graphics_surface, IDirectDraw7* direct_draw, IDirectDrawPalette* direct_palette)
	{
		DDSURFACEDESC2 surfaceDescription = { };
		surfaceDescription.dwSize = sizeof(DDSURFACEDESC2);
		surfaceDescription.dwFlags = DDSD_CAPS;
		surfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

		HRESULT result = direct_draw->CreateSurface(&surfaceDescription, graphics_surface, NULL);
		assert(SUCCEEDED(result));

		//result = (*graphics_surface)->SetPalette(direct_palette);
		//assert(SUCCEEDED(result));
	}

	void Init(WindowHandle window_handle, int width, int height)
	{
		InitDirectDraw(&GraphicsCardContext, window_handle, width, height);
		InitPalete(GraphicsCardContext, &GraphicsPalette);
		InitSurface(&GraphicsSurface, GraphicsCardContext, GraphicsPalette);
	}


	void LockVideoMemory()
	{
		DDSURFACEDESC2 surfaceDesc = { };
		surfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

		HRESULT result = GraphicsSurface->Lock(NULL, &surfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
		assert(SUCCEEDED(result));

		VideoBuffer.Pitch = surfaceDesc.lPitch;
		VideoBuffer.Width = surfaceDesc.dwWidth;
		VideoBuffer.Height = surfaceDesc.dwHeight;
		VideoBuffer.VideoMemory = surfaceDesc.lpSurface;
	}

	void Plot32(int x, int y, int color)
	{
		uint32_t * video_memory = (uint32_t*)VideoBuffer.VideoMemory;
		video_memory[x + y * (VideoBuffer.Pitch >> 2)] = color;
	}

	void GetVideoMemoryDimension(int& w, int& h)
	{
		DDSURFACEDESC2 surfaceDesc = { };
		surfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

		HRESULT result = GraphicsSurface->GetSurfaceDesc(&surfaceDesc);
		assert(SUCCEEDED(result));

		w = surfaceDesc.dwWidth;
		h = surfaceDesc.dwHeight;
	}

	void UnlockVideoMemory() 
	{
		GraphicsSurface->Unlock(NULL);
		VideoBuffer = { };
	}

	void Shutdown()
	{
		RELEASE_COM(GraphicsSurface);
		RELEASE_COM(GraphicsPalette);
		RELEASE_COM(GraphicsCardContext)
	}

}}