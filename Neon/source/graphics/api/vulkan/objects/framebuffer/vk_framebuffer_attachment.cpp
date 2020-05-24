#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer_attachment.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

namespace Neon
{
	namespace Graphics
	{
		VKFramebufferAttachment::VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{

		}
		VKFramebufferAttachment::~VKFramebufferAttachment()
		{


		}

		VKFramebufferAttachment::VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, VkImage _image)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
			, m_Image(_image)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType							= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image							= m_Image;
			createInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format							= VK_FORMAT_R8G8B8A8_UNORM;
			createInfo.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel	= 0;
			createInfo.subresourceRange.levelCount		= 1;
			createInfo.subresourceRange.baseArrayLayer	= 0;
			createInfo.subresourceRange.layerCount		= 1;
		
			VK_ThrowIfFailed(vkCreateImageView(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &createInfo, nullptr, &m_ImageView));
		}
	}
}