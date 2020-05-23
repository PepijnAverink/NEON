#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		VKFramebufferAttachment::VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{

		}
		VKFramebufferAttachment::VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, VkImage _image)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{

		}
	}
}