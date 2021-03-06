#pragma once
#include "./graphics/objects/command/command_buffer.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12CommandBuffer final : public CommandBuffer
		{
		public:
			DX12CommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor);
			virtual ~DX12CommandBuffer();

			virtual void StartRecording() override;
			virtual void EndRecording()	  override;

			virtual void Reset() override;

			// Commands
			virtual void SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) override;

			virtual void BindVertexBuffer(VertexBuffer* _vertexBuffer) const override;
			virtual void BindIndexBuffer(IndexBuffer* _indexBuffer) const override;

			virtual void BindComputeShader(ComputeShader* _computeShader) override;
			virtual void BindComputeBuffer(ComputeBuffer* _computeBuffer) override;
			virtual void DispatchCompute(const uint32_t _x, const uint32_t _y, const uint32_t _z) override;

			virtual void BindTexture(Texture2D* _texture, uint32_t _bindPoint) const override;
			virtual void BindTexture(FramebufferAttachment* _framebufferAttachment, uint32_t _bindPoint) const override;

			virtual void SetViewport(Viewport* _viewport) const override;
			virtual void SetScissor(Scissor* _scissor) const override;

			virtual void ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const override;

			virtual void BeginRenderpass(Framebuffer* _framebuffer) const override;
			virtual void EndRenderpass() const override;

			virtual void TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const override;

			virtual void DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const override;
		//	virtual void Draw();

		private:
			friend class DX12CommandQueue;
			friend class DX12VertexBuffer;
			friend class DX12IndexBuffer;
			friend class DX12GraphicsContext;
			ID3D12GraphicsCommandList* m_CommandListObj;
		};
	}
}