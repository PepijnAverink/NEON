#pragma once
#include "./graphics/graphics_driver.h"
#include "./graphics/objects/command/command_queue_layout.h"

namespace Neon
{
	namespace Core { class Window; }
	namespace Graphics
	{
		class GraphicsContextDescriptor
		{
		public:
			GraphicsContextDescriptor() = default;

			Core::Window*		Window;
			GraphicsAPI			GraphicsApi;

			CommandQueueLayout	QueueLayout;
		};
	}
}