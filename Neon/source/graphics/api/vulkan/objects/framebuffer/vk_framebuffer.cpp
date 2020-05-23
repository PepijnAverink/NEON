#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer.h"

namespace Neon
{
	namespace Graphics
	{
		// TODO:: Support depth texture
		VKFramebuffer::VKFramebuffer(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass)
			: Framebuffer(_framebufferDescriptor)
		{
		//	VkFramebufferCreateInfo framebufferInfo{};
		//	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		//	framebufferInfo.renderPass = m_RenderPass;
		//	framebufferInfo.attachmentCount = 1;
		//	framebufferInfo.pAttachments = attachments;
		//	framebufferInfo.width = 1280;
		//	framebufferInfo.height = 720;
		//	framebufferInfo.layers = 1;
		//
		//	if (vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
		//		throw std::runtime_error("failed to create framebuffer!");
		//	}
		}

		void VKFramebuffer::AddAttachment(FramebufferAttachment* _framebufferAttachment)
		{

		}
	}
}