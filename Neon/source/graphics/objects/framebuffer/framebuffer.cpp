#include "./graphics/objects/framebuffer/framebuffer.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		Framebuffer* Framebuffer::Create(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKFramebuffer(_framebufferDescriptor, _graphicsPipeline);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11Framebuffer(_framebufferDescriptor, _graphicsPipeline);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Framebuffer(_framebufferDescriptor, _graphicsPipeline);

			printf("[ERROR] Framebuffer::Create() No valid api abstraction for found for object: Framebuffer");
			return nullptr;
		}

		Framebuffer::Framebuffer(const FramebufferDescriptor* _framebufferDescriptor)
			: m_AttachmentCount(_framebufferDescriptor->AttachmentCount)
			, m_DepthStencilActive(_framebufferDescriptor->DepthAttachment)
		{

		}
	}
}