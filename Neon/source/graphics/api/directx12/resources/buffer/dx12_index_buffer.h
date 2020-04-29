#pragma once
#include "./graphics/resources/buffer/index_buffer.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12IndexBuffer : public IndexBuffer
		{
		public:
			DX12IndexBuffer(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor);
			virtual ~DX12IndexBuffer();

			virtual void* Map()   const override;
			virtual void  Unmap() const override;

		private:
			friend class DX12CommandBuffer;
			friend class DX12GraphicsContext; // TODO:: remove me
			ID3D12Resource*			m_IndexBuffer;
			D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		};
	}
}