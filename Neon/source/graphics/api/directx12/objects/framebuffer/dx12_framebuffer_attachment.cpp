#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"

namespace Neon
{
	namespace Graphics
	{
		DX12FramebufferAttachment::DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{
			if (m_Type == FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_DEPTH_STENCIL)
			{
				D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
				depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
				depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
				depthOptimizedClearValue.DepthStencil.Stencil = 0;

				DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, _framebufferAttachmentDescriptor->Width, _framebufferAttachmentDescriptor->Height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					&depthOptimizedClearValue,
					IID_PPV_ARGS(&m_Image)
				);
			}
		}

		DX12FramebufferAttachment::~DX12FramebufferAttachment()
		{
			m_Image->Release();
		}

		DX12FramebufferAttachment::DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor, ID3D12Resource* _image)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
			, m_Image(_image)
		{ }
	}
}