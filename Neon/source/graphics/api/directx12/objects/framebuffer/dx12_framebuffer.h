#pragma once
#include "./graphics/objects/framebuffer/framebuffer.h"

#include <d3d12.h>
#include <DX12/d3dx12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12Framebuffer : public Framebuffer
		{
		public:
			DX12Framebuffer(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass);

			CD3DX12_CPU_DESCRIPTOR_HANDLE GetAttachmentHandle() const { return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }
			CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilHandle() const { return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_DepthDescriptorHeap->GetCPUDescriptorHandleForHeapStart()); }

		private:
			virtual void AddAttachment(FramebufferAttachment* _framebufferAttachment) override;

			ID3D12DescriptorHeap* m_DescriptorHeap;
			int					  m_DescriptorSize;

			ID3D12DescriptorHeap* m_DepthDescriptorHeap;
		};
	}
}