#include "./graphics/api/vulkan/resources/buffer/vk_index_buffer.h"
#include "./graphics/api/vulkan/resources/memory/vk_memory_pool.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

namespace Neon
{
	namespace Graphics
	{
		VKIndexBuffer::VKIndexBuffer(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor)
			: IndexBuffer(_indexBufferDescriptor)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size			= _indexBufferDescriptor->Size;
			bufferInfo.usage		= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			bufferInfo.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

			VK_ThrowIfFailed(vkCreateBuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &bufferInfo, nullptr, &m_IndexBufferObj));

			// Associate buffer with memory
			vkBindBufferMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_IndexBufferObj, NEON_CAST(VKMemoryPool*, _indexBufferDescriptor->IMemoryPool)->m_memoryObj, 0);

			void* data = Map();

			memcpy(data, _indexBufferDescriptor->Indices, _indexBufferDescriptor->Size);

			Unmap();
		}

		VKIndexBuffer::~VKIndexBuffer()
		{
			vkDestroyBuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_IndexBufferObj, nullptr);
		}

		void* VKIndexBuffer::Map() const
		{
			void* data;
			vkMapMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), NEON_CAST(VKMemoryPool*, m_MemoryPool)->m_memoryObj, 0, m_Size, 0, &data);
			return data;
		}

		void VKIndexBuffer::Unmap() const
		{
			vkUnmapMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), NEON_CAST(VKMemoryPool*, m_MemoryPool)->m_memoryObj);
		}
	}
}