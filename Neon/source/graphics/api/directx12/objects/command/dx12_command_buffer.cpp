#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"
#include "./graphics/api/directx12/objects/command/dx12_command_pool.h"
#include "./graphics/api/directx12/objects/command/dx12_command_buffer_type.h"

#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment_transition_state.h"
#include "./graphics/objects/framebuffer/framebuffer_clear_flags.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/dx12_error.h"

#include "./graphics/api/directx12/objects/command_generic/dx12_viewport.h"
#include "./graphics/api/directx12/objects/command_generic/dx12_scissor.h"

#include "./graphics/api/directx12/pipeline/dx12_graphics_pipeline.h"
#include "./graphics/api/directx12/resources/buffer/dx12_vertex_buffer.h"
#include "./graphics/api/directx12/resources/buffer/dx12_index_buffer.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		// TODO:: abstract buffer type
		DX12CommandBuffer::DX12CommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor)
			: CommandBuffer(_commandBufferDescriptor)
		{
			DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommandList(0, GetDX12CommandBufferType(_commandBufferDescriptor->Type),
				NEON_CAST(DX12CommandPool*, _commandBufferDescriptor->CommandPool)->m_CommandAllocator, NULL, IID_PPV_ARGS(&m_CommandListObj));
			DX12_ThrowIfFailed(m_CommandListObj->Close()); // CommandLiss are created in a recording state
		}

		DX12CommandBuffer::~DX12CommandBuffer()
		{
			m_CommandListObj->Release();
		}

		void DX12CommandBuffer::StartRecording()
		{
			auto commandPool = NEON_CAST(DX12CommandPool*, m_CommandPool);
			m_CommandListObj->Reset(commandPool->m_CommandAllocator, NULL);
		}

		void DX12CommandBuffer::EndRecording()
		{
			DX12_ThrowIfFailed(m_CommandListObj->Close());
		}

		void DX12CommandBuffer::Reset()
		{
			m_CommandPool->Reset();

		//	auto commandPool = NEON_CAST(DX12CommandPool*, m_CommandPool);
		//	m_CommandListObj->Reset(commandPool->m_CommandAllocator, NULL);
		//	DX12_ThrowIfFailed(m_CommandListObj->Close());
		}

		void DX12CommandBuffer::SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline)
		{
			m_CommandListObj->SetPipelineState(NEON_CAST(DX12GraphicsPipeline*, _graphicsPipeline)->m_PipelineStateObject);

			// Dont set the root signature here
			m_CommandListObj->SetGraphicsRootSignature(NEON_CAST(DX12GraphicsPipeline*, _graphicsPipeline)->m_RootSignature);

			m_CommandListObj->IASetPrimitiveTopology(NEON_CAST(DX12GraphicsPipeline*, _graphicsPipeline)->m_InternalTopology);
		}

		void DX12CommandBuffer::BindVertexBuffer(VertexBuffer* _vertexBuffer) const
		{
			m_CommandListObj->IASetVertexBuffers(0, 1, &NEON_CAST(DX12VertexBuffer*, _vertexBuffer)->m_VertexBufferView);
		}

		void DX12CommandBuffer::BindIndexBuffer(IndexBuffer* _indexBuffer) const
		{
			m_CommandListObj->IASetIndexBuffer(&NEON_CAST(DX12IndexBuffer*, _indexBuffer)->m_IndexBufferView);
		}

		void DX12CommandBuffer::BindComputeShader(ComputeShader* _computeShader)
		{
		}

		void DX12CommandBuffer::BindComputeBuffer(ComputeBuffer* _computeBuffer)
		{
		}

		void DX12CommandBuffer::DispatchCompute(const uint32_t _x, const uint32_t _y, const uint32_t _z)
		{
		}

		void DX12CommandBuffer::BindTexture(Texture2D* _texture, uint32_t _bindPoint) const
		{

		}

		void DX12CommandBuffer::BindTexture(FramebufferAttachment* _framebufferAttachment, uint32_t _bindPoint) const
		{

		}

		void DX12CommandBuffer::SetViewport(Viewport* _viewport) const
		{
			m_CommandListObj->RSSetViewports(1, &NEON_CAST(DX12Viewport*, _viewport)->m_InternalViewport);
		}

		void DX12CommandBuffer::SetScissor(Scissor* _scissor) const
		{
			m_CommandListObj->RSSetScissorRects(1, &NEON_CAST(DX12Scissor*, _scissor)->m_InternalScissor);
		}

		void DX12CommandBuffer::ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const
		{
			// TODO:: Support clearning of whole framebuffer
			if (_flags & NEON_CLEAR_COLOR_BIT)
				m_CommandListObj->ClearRenderTargetView(NEON_CAST(DX12Framebuffer*, _framebuffer)->GetAttachmentHandle(), _color, 0, nullptr);

			if (_flags & NEON_CLEAR_DEPTH_STENCIL_BIT) // No further checking, user specified depth clearing
				m_CommandListObj->ClearDepthStencilView(NEON_CAST(DX12Framebuffer*, _framebuffer)->GetDepthStencilHandle(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		}

		void DX12CommandBuffer::BeginRenderpass(Framebuffer* _framebuffer) const
		{
			m_CommandListObj->OMSetRenderTargets(_framebuffer->GetAttachmentCount(), &NEON_CAST(DX12Framebuffer*, _framebuffer)->GetAttachmentHandle(), FALSE, _framebuffer->IsDepthStencilActive() ? &NEON_CAST(DX12Framebuffer*, _framebuffer)->GetDepthStencilHandle() : nullptr);
		}

		void DX12CommandBuffer::EndRenderpass() const
		{

		}

		void DX12CommandBuffer::TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const
		{
			m_CommandListObj->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(NEON_CAST(DX12FramebufferAttachment*, _framebufferAttachment)->m_Image, 
				GetDX12FramebufferTransitionState(_fromState), GetDX12FramebufferTransitionState(_toState)));
		}

		void DX12CommandBuffer::DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const
		{
			m_CommandListObj->DrawIndexedInstanced(_indexCount, 1, _indexOffset, _vertexOffset, 0);

		}
	}
}