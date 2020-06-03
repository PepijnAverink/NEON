#include "./graphics/api/directx11/objects/command/dx11_command_buffer.h"
#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer.h"
#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer_attachment.h"
#include "./graphics/api/directx11/resources/buffer/dx11_vertex_buffer.h"
#include "./graphics/api/directx11/resources/buffer/dx11_index_buffer.h"
#include "./graphics/api/directx11/pipeline/dx11_graphics_pipeline.h"
#include "./graphics/api/directx11/objects/command_generic/dx11_viewport.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX11CommandBuffer::DX11CommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor)
			: CommandBuffer(_commandBufferDescriptor)
		{
			// Create the defered context
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateDeferredContext(0, &m_DeferedContext));
		}

		DX11CommandBuffer::~DX11CommandBuffer()
		{ }

		void DX11CommandBuffer::StartRecording()
		{ }

		void DX11CommandBuffer::EndRecording()
		{
			DX11_ThrowIfFailed(m_DeferedContext->FinishCommandList(FALSE, &m_DeferedListObj));
		}

		void DX11CommandBuffer::Reset()
		{
			if (m_DeferedListObj != nullptr)
			{
				m_DeferedListObj->Release();
				m_DeferedListObj = nullptr;
			}
		}

		void DX11CommandBuffer::SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline)
		{
			DX11GraphicsPipeline* pipeline = NEON_CAST(DX11GraphicsPipeline*, _graphicsPipeline);

			// Set the vertex input layout.
			m_DeferedContext->IASetInputLayout(pipeline->m_InputLayout);

			// Bind the shader
			m_DeferedContext->VSSetShader(pipeline->m_InternalShader->m_VertexShader, 0, 0);
			m_DeferedContext->PSSetShader(pipeline->m_InternalShader->m_FragmentShader, 0, 0);

			m_DeferedContext->OMSetDepthStencilState(pipeline->m_DepthStencilState, 1);
			m_DeferedContext->RSSetState(pipeline->m_RasterState);

			m_DeferedContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		void DX11CommandBuffer::BindVertexBuffer(VertexBuffer* _vertexBuffer) const
		{
			unsigned int stride = _vertexBuffer->GetVertexStride();
			unsigned int offset = 0;

			// Bind the buffer
			m_DeferedContext->IASetVertexBuffers(0, 1, &NEON_CAST(DX11VertexBuffer*, _vertexBuffer)->m_VertexBuffer, &stride, &offset);
		}

		void DX11CommandBuffer::BindIndexBuffer(IndexBuffer* _indexBuffer) const
		{
			DX11IndexBuffer* index = NEON_CAST(DX11IndexBuffer*, _indexBuffer);
			m_DeferedContext->IASetIndexBuffer(index->m_IndexBuffer, index->m_IndexBufferFormat, 0);
		}

		void DX11CommandBuffer::BindTexture(Texture2D* _texture, uint32_t _bindPoint) const
		{

		}

		void DX11CommandBuffer::BindTexture(FramebufferAttachment* _framebufferAttachment, uint32_t _bindPoint) const
		{

		}

		void DX11CommandBuffer::SetViewport(Viewport* _viewport) const
		{
			m_DeferedContext->RSSetViewports(1, &NEON_CAST(DX11Viewport*, _viewport)->m_InternalViewport);
		}

		void DX11CommandBuffer::SetScissor(Scissor* _scissor) const
		{

		}

		void DX11CommandBuffer::ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const
		{
			if (_flags & NEON_CLEAR_COLOR_BIT)
				m_DeferedContext->ClearRenderTargetView(NEON_CAST(DX11FramebufferAttachment*, _framebuffer->GetAttachment(0))->m_ImageView, _color);
		}

		void DX11CommandBuffer::BeginRenderpass(Framebuffer* _framebuffer) const
		{
			m_DeferedContext->OMSetRenderTargets(1, &NEON_CAST(DX11FramebufferAttachment*, _framebuffer->GetAttachment(0))->m_ImageView, nullptr);
		}

		void DX11CommandBuffer::EndRenderpass() const
		{

		}

		void DX11CommandBuffer::TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const
		{
		}

		void DX11CommandBuffer::DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const
		{
			m_DeferedContext->DrawIndexed(_indexCount, 0, 0);
		}
	}
}