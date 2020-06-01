#pragma once
#include "./graphics/resources/buffer/vertex_buffer.h"

#include <D3D11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11VertexBuffer : public VertexBuffer
		{
		public:
			DX11VertexBuffer(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor);
			virtual ~DX11VertexBuffer();

			virtual void* Map()   const override;
			virtual void  Unmap() const override;

		private:
			friend class DX11CommandBuffer;
			ID3D11Buffer* m_VertexBuffer;
		};
	}
}