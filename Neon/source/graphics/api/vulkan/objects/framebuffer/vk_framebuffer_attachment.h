#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKFramebufferAttachment : public FramebufferAttachment
		{
		public:
			VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);

		private:
			friend class VKSwapchain;
			VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, VkImage _image);

			VkImage m_Image;
		};
	}
}