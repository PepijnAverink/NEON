#pragma once
#include "./graphics/objects/framebuffer/framebuffer_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class Framebuffer
		{
		public:
			Framebuffer* Create(const Framebuffer* _framebufferDescriptor);

		private:
			Framebuffer(const Framebuffer* _framebufferDescriptor);
		};
	}
}