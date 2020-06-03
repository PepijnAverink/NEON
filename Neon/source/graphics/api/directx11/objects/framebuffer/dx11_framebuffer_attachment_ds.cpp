#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer_attachment_ds.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX11FramebufferAttachmentDS::DX11FramebufferAttachmentDS(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachmentDS(_framebufferAttachmentDescriptor)
		{
			D3D11_TEXTURE2D_DESC depthBufferDesc = {};
			depthBufferDesc.Width				= _framebufferAttachmentDescriptor->Width;
			depthBufferDesc.Height				= _framebufferAttachmentDescriptor->Height;
			depthBufferDesc.MipLevels			= 1;
			depthBufferDesc.ArraySize			= 1;
			depthBufferDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthBufferDesc.SampleDesc.Count	= 1;
			depthBufferDesc.SampleDesc.Quality	= 0;
			depthBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
			depthBufferDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
			depthBufferDesc.CPUAccessFlags		= 0;
			depthBufferDesc.MiscFlags			= 0;

			// Craete texture
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateTexture2D(&depthBufferDesc, NULL, &m_DepthStencilBuffer));

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			// Create the depth stencil view.
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateDepthStencilView(m_DepthStencilBuffer, &depthStencilViewDesc, &m_DepthStencilView));

		}
	}
}