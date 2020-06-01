#include <pch.h>
#include <ddraw.h>

#include "../graphics_core.h"
#include "graphics.h"

struct back_buffer
{
	int Width;
	int Height;
	// Pitch32 because pixel consists of 32bit
	int Pitch32;
	int MemorySize;
	void *Memory;
};

#define RELEASE_COM(x) if (x) { x->Release(); x = nullptr; }

internal back_buffer VideoBuffer;

internal IDirectDraw7 *GraphicsCardContext = nullptr;
internal IDirectDrawSurface7 *PrimarySurface = nullptr;
internal IDirectDrawSurface7 *SecondarySurface = nullptr;

#define DEFAULT_BPP_MODE_BIT 32
#define DEFAULT_BPP_MODE_BYTE 4

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

			result = (*direct_draw)->SetDisplayMode(width, height, DEFAULT_BPP_MODE_BIT, 0, 0);
			assert(SUCCEEDED(result));
		}
	}

	internal void InitSurfaceWithBackBuffer(
		IDirectDrawSurface7** graphics_surface,
		IDirectDrawSurface7** back_surface,
		IDirectDraw7* direct_draw)
	{
		DDSURFACEDESC2 surfaceDescription = { };
		DDRAW_INIT_STRUCT(surfaceDescription)

		surfaceDescription.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		surfaceDescription.dwBackBufferCount = 1;
		surfaceDescription.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;

		HRESULT result = direct_draw->CreateSurface(&surfaceDescription, graphics_surface, NULL);
		assert(SUCCEEDED(result));

		DDSCAPS2 ddCaps = {};
		ddCaps.dwCaps = DDSCAPS_BACKBUFFER;

		result = (*graphics_surface)->GetAttachedSurface(&ddCaps, back_surface);
		assert(SUCCEEDED(result));
	}

	void Init(WindowHandle windowHandle, graphics_setting graphicsSetting)
	{
		InitDirectDraw(&GraphicsCardContext, 
			windowHandle, 
			graphicsSetting.ResolutionWidth, 
			graphicsSetting.ResolutionHeight,
			graphicsSetting.IsWindowed);

		InitSurfaceWithBackBuffer(&PrimarySurface, &SecondarySurface, GraphicsCardContext);
	}

	void StartDraw()
	{
		DDSURFACEDESC2 surfaceDesc = { };
		DDRAW_INIT_STRUCT(surfaceDesc)

		HRESULT result = SecondarySurface->Lock(NULL, &surfaceDesc, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
		assert(SUCCEEDED(result));

		VideoBuffer.Pitch32 = (surfaceDesc.lPitch >> 2); // shift >> 2 - because color depth 32bit
		VideoBuffer.Width = surfaceDesc.dwWidth;
		VideoBuffer.Height = surfaceDesc.dwHeight;
		VideoBuffer.Memory = (uint32_t*)surfaceDesc.lpSurface;
		VideoBuffer.MemorySize = surfaceDesc.lPitch * surfaceDesc.dwHeight;

		memset(VideoBuffer.Memory, 0, VideoBuffer.MemorySize);
	}

	void Plot32(int x, int y, int color)
	{
		uint32_t * video_memory = (uint32_t*)VideoBuffer.Memory;
		video_memory[x + y * VideoBuffer.Pitch32] = color;
	}

	void GetVideoMemoryDimension(int& w, int& h)
	{
		DDSURFACEDESC2 surfaceDesc = { };
		DDRAW_INIT_STRUCT(surfaceDesc)

		HRESULT result = PrimarySurface->GetSurfaceDesc(&surfaceDesc);
		assert(SUCCEEDED(result));

		w = surfaceDesc.dwWidth;
		h = surfaceDesc.dwHeight;
	}

	void EndDraw()
	{
		SecondarySurface->Unlock(NULL);

		while (FAILED(PrimarySurface->Flip(NULL, DDFLIP_WAIT)));

		VideoBuffer = { };
	}

	void Shutdown()
	{
		RELEASE_COM(SecondarySurface);
		RELEASE_COM(PrimarySurface);
		RELEASE_COM(GraphicsCardContext)
	}

}}