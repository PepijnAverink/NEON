#pragma once
#include "./graphics/resources/buffer/compute_buffer_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class ComputeBuffer
		{
		public:
			static ComputeBuffer* Create(const ComputeBufferDescriptor* _computeBufferDescriptor);
			virtual ~ComputeBuffer() {}

			virtual void* GetData() = 0;

		protected:
			ComputeBuffer(const ComputeBufferDescriptor* _computeBufferDescriptor);

			uint32_t m_Size;
		};
	}
}