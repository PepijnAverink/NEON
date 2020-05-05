#pragma once
#include "./graphics/objects/framebuffer/framebuffer_layout.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class RenderpassDescriptor
		{
		public:
			RenderpassDescriptor() = default;

			std::string			Name;
			FramebufferLayout   Layout;
		};
	}
}