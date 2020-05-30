#pragma once
#include "./graphics/graphics_driver.h"

namespace Neon
{
	namespace Core { class Window; }
	namespace Graphics
	{
		class GraphicsContextDescriptor
		{
		public:
			GraphicsContextDescriptor() = default;

			Core::Window*	Window;
			GraphicsAPI		GraphicsApi;
		};
	}
}