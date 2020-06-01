#pragma once
#include "./graphics/resources/buffer/index_buffer.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11IndexBuffer : public IndexBuffer
		{
		public:
			DX11IndexBuffer(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor);
			virtual ~DX11IndexBuffer();

			virtual void* Map()   const override;
			virtual void  Unmap() const override;

		private:
			friend class DX11CommandBuffer;
			ID3D11Buffer* m_IndexBuffer;
			DXGI_FORMAT   m_IndexBufferFormat;
		};
	}
}