#pragma once
#include "./graphics/objects/swapchain/back_buffer_format.h"
#include "./graphics/objects/swapchain/graphics_surface.h"
#include "./graphics/hardware/adapter/video_adapter.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class SwapchainDescriptor
		{
		public:
			SwapchainDescriptor() = default;

			std::string Name;

			int Width;
			int Height;
			int BackBufferCount;

			BackBufferFormat Format;
			GraphicsSurface* Surface;

			VideoAdapter* Adapter;
		};
	}
}