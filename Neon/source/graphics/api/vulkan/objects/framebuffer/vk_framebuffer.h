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
			VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline);
			virtual ~VKFramebuffer();

		private:
			virtual void AddAttachment(FramebufferAttachment* _framebufferAttachment) override;

			friend class VKGraphicsContext; // Remove me
			VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline, VkImageView* _attachments); // Remove me

			VkFramebuffer m_FramebufferObj;
		};
	}
}