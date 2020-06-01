#include "./graphics/api/directx11/objects/command/dx11_command_buffer.h"
#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer.h"
#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer_attachment.h"
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

		void DX11CommandBuffer::StartRecording() const
		{

		}

		void DX11CommandBuffer::EndRecording()
		{
			DX11_ThrowIfFailed(m_DeferedContext->FinishCommandList(FALSE, &m_DeferedListObj));
		}

		void DX11CommandBuffer::Reset() const
		{

		}

		void DX11CommandBuffer::SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline)
		{

		}

		void DX11CommandBuffer::SetVertexBuffer(VertexBuffer* _vertexBuffer) const
		{

		}

		void DX11CommandBuffer::SetIndexBuffer(IndexBuffer* _indexBuffer) const
		{

		}

		void DX11CommandBuffer::SetViewport(Viewport* _viewport) const
		{

		}

		void DX11CommandBuffer::SetScissor(Scissor* _scissor) const
		{

		}

		void DX11CommandBuffer::ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const
		{
			if (_flags & NEON_CLEAR_COLOR_BIT)
				DX11GraphicsContext::GetInstance()->GetGraphicsDeviceContext()->ClearRenderTargetView(NEON_CAST(DX11FramebufferAttachment*, _framebuffer->GetAttachment(0))->m_ImageView, _color);
		}

		void DX11CommandBuffer::BeginRenderpass(Framebuffer* _framebuffer) const
		{
			DX11GraphicsContext::GetInstance()->GetGraphicsDeviceContext()->OMSetRenderTargets(1, &NEON_CAST(DX11FramebufferAttachment*, _framebuffer->GetAttachment(0))->m_ImageView, nullptr);
		}

		void DX11CommandBuffer::EndRenderpass() const
		{

		}

		void DX11CommandBuffer::TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const
		{
		}

		void DX11CommandBuffer::DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const
		{

		}
	}
}