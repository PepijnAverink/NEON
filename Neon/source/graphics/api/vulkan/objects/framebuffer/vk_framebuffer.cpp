#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer.h"
#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer_attachment.h"
#include "./graphics/api/vulkan/pipeline/vk_graphics_pipeline.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./utilities/casting/casting_helper.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		// TODO:: Support depth texture
		VKFramebuffer::VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline)
			: Framebuffer(_framebufferDescriptor)
		{
			std::vector<VkImageView> attachments;
			for (int i = 0; i < m_AttachmentCount; ++i)
			{
				AddAttachment(_framebufferDescriptor->Attachments[i]);
				attachments.push_back(NEON_CAST(VKFramebufferAttachment*, _framebufferDescriptor->Attachments[i])->m_ImageView);
			}

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass		= NEON_CAST(VKGraphicsPipeline*, _graphicsPipeline)->m_RenderPass;
			framebufferInfo.attachmentCount = m_AttachmentCount;
			framebufferInfo.pAttachments	= attachments.data();
			framebufferInfo.width			= _framebufferDescriptor->Width;
			framebufferInfo.height			= _framebufferDescriptor->Height;
			framebufferInfo.layers			= 1;
		
			VK_ThrowIfFailed(vkCreateFramebuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &framebufferInfo, nullptr, &m_FramebufferObj));
		}

		VKFramebuffer::~VKFramebuffer()
		{
		
		}

		void VKFramebuffer::AddAttachment(FramebufferAttachment* _framebufferAttachment)
		{
			// Add attachment
			m_FramebufferAttachments.push_back(_framebufferAttachment);
		}

		VKFramebuffer::VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline, VkImageView* _attachments)
			: Framebuffer(_framebufferDescriptor)
		{
			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = NEON_CAST(VKGraphicsPipeline*, _graphicsPipeline)->m_RenderPass;
			framebufferInfo.attachmentCount = m_AttachmentCount;
			framebufferInfo.pAttachments = _attachments;
			framebufferInfo.width = _framebufferDescriptor->Width;
			framebufferInfo.height = _framebufferDescriptor->Height;
			framebufferInfo.layers = 1;

			VK_ThrowIfFailed(vkCreateFramebuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &framebufferInfo, nullptr, &m_FramebufferObj));
		}
	}
}