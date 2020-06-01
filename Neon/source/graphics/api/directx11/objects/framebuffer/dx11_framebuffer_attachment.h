#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11FramebufferAttachment : public FramebufferAttachment
		{
		public:
			DX11FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);
			virtual ~DX11FramebufferAttachment();

		private:
			friend class DX11Swapchain;
			friend class DX11CommandBuffer;
			DX11FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, ID3D11Texture2D* _image);

			ID3D11Texture2D*		m_Image;
			union
			{
				ID3D11RenderTargetView*  m_ImageView;
				ID3D11DepthStencilView*  m_DepthStencilView;
			};
		};
	}
}