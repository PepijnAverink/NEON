#pragma once
#include "./graphics/resources/texture/texture_format.h"

namespace Neon
{
	namespace Core { class Window; }
	namespace Graphics
	{
		class SwapchainDescriptor
		{
		public:
			SwapchainDescriptor() = default;

			int Width;
			int Height;
			int BackBufferCount;

			TextureFormat BackBufferFormat;
			Core::Window* Window;

		};
	}
}