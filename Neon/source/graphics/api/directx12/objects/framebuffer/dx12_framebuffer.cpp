#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/dx12_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX12Framebuffer::DX12Framebuffer(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass)
			: Framebuffer(_framebufferDescriptor)
		{
			// Setup RV DescriptorHeap
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors  = m_AttachmentCount;
			rtvHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			DX12_ThrowIfFailed(DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_DescriptorHeap)));

			// Get the size of a descriptor in this heap 
			m_DescriptorSize = DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			// Setup depth descriptor heap
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors	= 1;
			dsvHeapDesc.Type			= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DX12_ThrowIfFailed(DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DepthDescriptorHeap)));
		}

		void DX12Framebuffer::AddAttachment(FramebufferAttachment* _framebufferAttachment)
		{			
			if (_framebufferAttachment->GetType() != FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_DEPTH_STENCIL)
			{
				// get a handle to the first descriptor in the descriptor heap
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(), m_FramebufferAttachments.size(), m_DescriptorSize);

				auto image = NEON_CAST(DX12FramebufferAttachment*, _framebufferAttachment)->m_Image;
				DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateRenderTargetView(image, nullptr, rtvHandle);
			}
			else
			{
				// TODO:: Abtract depth format
				D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
				depthStencilDesc.Format			= DXGI_FORMAT_D32_FLOAT;
				depthStencilDesc.ViewDimension	= D3D12_DSV_DIMENSION_TEXTURE2D;
				depthStencilDesc.Flags			= D3D12_DSV_FLAG_NONE;

				auto image = NEON_CAST(DX12FramebufferAttachment*, _framebufferAttachment)->m_Image;
				DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateDepthStencilView(image, &depthStencilDesc, m_DepthDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
			}

			// Add attachment
			m_FramebufferAttachments.push_back(_framebufferAttachment);
		}
	}
}