#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer.h"

namespace Neon
{
	namespace Graphics
	{
		DX11Framebuffer::DX11Framebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline)
			: Framebuffer(_framebufferDescriptor)
		{

		}

		void DX11Framebuffer::AddAttachment(FramebufferAttachment* _framebufferAttachment)
		{

		}
	}
}