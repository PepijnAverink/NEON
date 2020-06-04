#pragma once
#include "./graphics/resources/buffer/compute_buffer.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11ComputeBuffer : public ComputeBuffer
		{
		public:
			DX11ComputeBuffer(const ComputeBufferDescriptor* _computeBufferDescriptor);
			virtual ~DX11ComputeBuffer();

			virtual void* GetData() override;

		private:
			friend class DX11CommandBuffer;
			ID3D11Buffer*				m_ComputeBuffer;
			ID3D11UnorderedAccessView*	m_ComputeBufferView;
		};
	}
}