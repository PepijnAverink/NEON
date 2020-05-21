#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer.h"

namespace Neon
{
	namespace Graphics
	{
		VKFramebuffer::VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass)
			: Framebuffer(_framebufferDescriptor)
		{

		}

		void VKFramebuffer::AddAttachment(FramebufferAttachment* _framebufferAttachment)
		{

		}
	}
}