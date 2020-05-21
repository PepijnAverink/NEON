#pragma once
#include "./graphics/objects/framebuffer/framebuffer.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKFramebuffer : public Framebuffer
		{
		public:
			VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass);

		private:
			virtual void AddAttachment(FramebufferAttachment* _framebufferAttachment) override;

			VkFramebuffer m_FramebufferObj;
		};
	}
}