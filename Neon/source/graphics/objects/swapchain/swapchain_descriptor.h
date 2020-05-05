#pragma once
#include "./graphics/objects/swapchain/back_buffer_format.h"

#include <string>

namespace Neon
{
	namespace Core { class Window; }
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
			Core::Window*    Window;

		};
	}
}