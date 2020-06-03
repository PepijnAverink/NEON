#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_ds.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11FramebufferAttachmentDS : public FramebufferAttachmentDS
		{
		public:
			DX11FramebufferAttachmentDS(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);

		private:
			ID3D11Texture2D*		 m_DepthStencilBuffer;
			ID3D11DepthStencilView*  m_DepthStencilView;
		};
	}
}