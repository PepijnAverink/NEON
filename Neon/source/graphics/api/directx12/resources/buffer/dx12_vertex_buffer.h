#pragma once
#include "./graphics/resources/buffer/vertex_buffer.h"

#include <D3D12.h>
#include <wrl/client.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12VertexBuffer : public VertexBuffer
		{
		public:
			DX12VertexBuffer(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor);
			virtual ~DX12VertexBuffer();

			virtual void* Map()   const override;
			virtual void  Unmap() const override;

		private:
			friend class DX12GraphicsContext; // TODO:: remove me
			ID3D12Resource*						   m_VertexBuffer;
			D3D12_VERTEX_BUFFER_VIEW			   m_VertexBufferView;
		};
	}
}