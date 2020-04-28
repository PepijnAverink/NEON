#include "./graphics/api/vulkan/resources/buffer/vk_index_buffer.h"

namespace Neon
{
	namespace Graphics
	{
		VKIndexBuffer::VKIndexBuffer(const IndexBufferDescriptor* _indexBufferDescriptor)
			: IndexBuffer(_indexBufferDescriptor)
		{

		}

		VKIndexBuffer::~VKIndexBuffer()
		{

		}

		void* VKIndexBuffer::Map() const
		{
			return nullptr;
		}

		void VKIndexBuffer::Unmap() const
		{

		}
	}
}