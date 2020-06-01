#pragma once

#include "platform.h"
#include "core/graphics/directx/direct_draw_context.hpp"

namespace core { namespace graphics {

	class DrawContextFactory
	{
	public:
		static IDrawContext* GetDrawContext(const graphics_setting& Setting, WindowHandle windowHandle)
		{
			switch (Setting.GraphicsApiType)
			{
				case GraphicsApiEnum::DirectX:
				{
					return new DirectDrawContext(
						windowHandle,
						Setting.ResolutionWidth,
						Setting.ResolutionHeight,
						Setting.IsWindowed
					);
				}
				default:
				{
					assert(false);
					return nullptr;
				}
			}
		}
	};

}}
