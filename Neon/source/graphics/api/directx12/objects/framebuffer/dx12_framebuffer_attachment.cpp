#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		DX12FramebufferAttachment::DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{

		}

		DX12FramebufferAttachment::DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, ID3D12Resource* _image)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
			, m_Image(_image)
		{ }
	}
}