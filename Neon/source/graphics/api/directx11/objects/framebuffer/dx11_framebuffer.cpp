#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer.h"

namespace Neon
{
	namespace Graphics
	{
		DX11Framebuffer::DX11Framebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline)
			: Framebuffer(_framebufferDescriptor)
		{
			// Add the attachments
			for (int i = 0; i < m_AttachmentCount; ++i)
				AddAttachment(_framebufferDescriptor->Attachments[i]);

			// Add depthStencil attechment if it is active
			if (m_DepthStencilActive)
				AddAttachment(_framebufferDescriptor->Attachments[m_AttachmentCount]);
		}

		void DX11Framebuffer::AddAttachment(FramebufferAttachment* _framebufferAttachment)
		{
			// Add attachment
			m_FramebufferAttachments.push_back(_framebufferAttachment);
		}
	}
}