#pragma once
#include "./graphics/objects/command/command_buffer.h"

#include <D3D11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11CommandBuffer : public CommandBuffer
		{
		public:
			DX11CommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor);
			virtual ~DX11CommandBuffer();

			virtual void StartRecording()  const override;
			virtual void EndRecording()	         override;

			virtual void Reset() const override;

			// Commands
			virtual void SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) override;

			virtual void SetVertexBuffer(VertexBuffer* _vertexBuffer) const override;
			virtual void SetIndexBuffer(IndexBuffer* _indexBuffer) const override;

			virtual void SetViewport(Viewport* _viewport) const override;
			virtual void SetScissor(Scissor* _scissor) const override;

			virtual void ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const override;

			virtual void BeginRenderpass(Framebuffer* _framebuffer) const override;
			virtual void EndRenderpass() const override;

			virtual void TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const override;

			virtual void DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const override;
			//	virtual void Draw();

		private:
			friend class DX11CommandQueue;
			ID3D11DeviceContext* m_DeferedContext;
			ID3D11CommandList*   m_DeferedListObj = NULL;
		};
	}
}