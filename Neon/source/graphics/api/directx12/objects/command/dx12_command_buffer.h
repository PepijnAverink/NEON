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

			virtual void StartRecording()  const override;
			virtual void EndRecording()	   const override;

			virtual void Reset() const override;

			// Commands
			virtual void SetViewport(const Viewport _viewport) const override;

			virtual void SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) const override;

			virtual void SetVertexBuffer(VertexBuffer* _vertexBuffer) const override;
			virtual void SetIndexBuffer(IndexBuffer* _indexBuffer) const override;

		private:
			friend class DX12CommandQueue;
			friend class DX12VertexBuffer;
			friend class DX12IndexBuffer;
			friend class DX12GraphicsContext;
			ID3D12GraphicsCommandList* m_CommandListObj;
		};
	}
}