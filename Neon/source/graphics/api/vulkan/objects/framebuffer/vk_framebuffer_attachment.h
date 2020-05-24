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
			virtual ~VKFramebufferAttachment();

		private:
			friend class VKSwapchain;
			VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, VkImage _image);

			friend class VKGraphicsContext; // Remove me
			friend class VKFramebuffer;
			friend class VKCommandBuffer;
			VkImage		m_Image;
			VkImageView m_ImageView;
		};
	}
}