#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12FramebufferAttachment : FramebufferAttachment
		{
		public:
			DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);

		private:
			friend class DX12Swapchain;
			DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, ID3D12Resource* _image);

			friend class DX12Framebuffer;
			friend class DX12GraphicsContext; // Remove me
			ID3D12Resource* m_Image;
		};
	}
}