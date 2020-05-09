#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		FramebufferAttachment* FramebufferAttachment::Create(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			//if (api == GraphicsAPI::VULKAN)
			//	return new VKCommandBuffer(_renderpassDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12FramebufferAttachment(_framebufferAttachmentDescriptor);

			printf("[ERROR] FramebufferAttachment::Create() No valid api abstraction for found for object: FramebufferAttachment");
			return nullptr;
		}

		FramebufferAttachment::FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: m_Type(_framebufferAttachmentDescriptor->Type)
		{
			
		}
	}
}