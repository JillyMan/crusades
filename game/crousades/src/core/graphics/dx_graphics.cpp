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

internal back_buffer VideoBuffer;

internal IDirectDraw7 *GraphicsCardContext = nullptr;
internal IDirectDrawSurface7 *PrimarySurface = nullptr;

internal IDirectDrawPalette *GraphicsPalette = nullptr;
internal PALETTEENTRY Palette[256];

#define DEFAULT_BPP_MODE 32

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0, \
sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

namespace core { namespace graphics {

	void InitDirectDraw(IDirectDraw7** direct_draw, WindowHandle window_handle, int width, int height, int windowed)
	{
		HRESULT result = DirectDrawCreateEx(NULL, (void**)&(*direct_draw), IID_IDirectDraw7, NULL);
		assert(SUCCEEDED(result));

		if (windowed)
		{
			result = (*direct_draw)->SetCooperativeLevel(window_handle, DDSCL_NORMAL);
			assert(SUCCEEDED(result));
		}
		else {
			result = (*direct_draw)->SetCooperativeLevel(window_handle,
				DDSCL_FULLSCREEN   |
				DDSCL_EXCLUSIVE    |
				DDSCL_ALLOWREBOOT  |
				DDSCL_MULTITHREADED
			);

			result = (*direct_draw)->SetDisplayMode(width, height, DEFAULT_BPP_MODE, 0, 0);
			assert(SUCCEEDED(result));
		}
	}

	internal void InitPalete(IDirectDraw7* direct_draw, IDirectDrawPalette** direct_palette)
	{
		for (int color = 1; color < 255; color++)
		{
			Palette[color].peRed = rand() % 256;
			Palette[color].peGreen = rand() % 256;
			Palette[color].peBlue = rand() % 256;
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

	void Init(WindowHandle window_handle, int width, int height, int windowed)
	{
		InitDirectDraw(&GraphicsCardContext, window_handle, width, height, windowed);
		InitPalete(GraphicsCardContext, &GraphicsPalette);
		InitSurface(&PrimarySurface, GraphicsCardContext, GraphicsPalette);

		DDPIXELFORMAT pixelFormat = {};
		pixelFormat.dwSize = sizeof(DDPIXELFORMAT);
		HRESULT result = PrimarySurface->GetPixelFormat(&pixelFormat);
		assert(SUCCEEDED(result));
	}


	void LockVideoMemory()
	{
		DDSURFACEDESC2 surfaceDesc = { };
		surfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

		HRESULT result = PrimarySurface->Lock(NULL, &surfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
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

		HRESULT result = PrimarySurface->GetSurfaceDesc(&surfaceDesc);
		assert(SUCCEEDED(result));

		w = surfaceDesc.dwWidth;
		h = surfaceDesc.dwHeight;
	}

	void ClearBuffer32(int color)
	{
		DDBLTFX blitterFx = { };
		blitterFx.dwSize = sizeof(blitterFx);

		blitterFx.dwFillColor = color;

		// ready to blt to surface
		PrimarySurface->Blt(NULL,         // ptr to dest rectangle
			NULL,                         // ptr to source surface, NA
			NULL,                         // ptr to source rectangle, NA
			DDBLT_COLORFILL | DDBLT_WAIT, // fill and wait
			&blitterFx);                  // ptr to DDBLTFX structure
	}

	void UnlockVideoMemory() 
	{
		PrimarySurface->Unlock(NULL);
		VideoBuffer = { };
	}

	void Shutdown()
	{
		RELEASE_COM(PrimarySurface);
		RELEASE_COM(GraphicsPalette);
		RELEASE_COM(GraphicsCardContext)
	}

}}