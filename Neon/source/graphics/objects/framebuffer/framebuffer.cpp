#include "./graphics/objects/framebuffer/framebuffer.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer.h"
#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		Framebuffer* Framebuffer::Create(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			//if (api == GraphicsAPI::VULKAN)
			//	return new VKCommandBuffer(_renderpassDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Framebuffer(_framebufferDescriptor, _renderpass);

			printf("[ERROR] Framebuffer::Create() No valid api abstraction for found for object: Framebuffer");
			return nullptr;
		}

		Framebuffer::Framebuffer(const FramebufferDescriptor* _framebufferDescriptor)
			: m_AttachmentCount(_framebufferDescriptor->AttachmentCount)
		{

		}
	}
}