#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer_attachment.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX11FramebufferAttachment::DX11FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{

		}

		DX11FramebufferAttachment::~DX11FramebufferAttachment()
		{
			m_Image->Release();
			m_ImageView->Release();
		}

		DX11FramebufferAttachment::DX11FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, ID3D11Texture2D* _image)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
			, m_Image(_image)
		{
			//Create our Render Target
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateRenderTargetView(m_Image, NULL, &m_ImageView));
		}
	}
}