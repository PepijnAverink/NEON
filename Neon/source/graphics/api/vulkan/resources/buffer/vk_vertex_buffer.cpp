#include "./graphics/api/vulkan/resources/buffer/vk_vertex_buffer.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/resources/memory/vk_memory_pool.h"

#include "./graphics/graphics_defines.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		VKVertexBuffer::VKVertexBuffer(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor)
			: VertexBuffer(_vertexBufferDescriptor)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size			= _vertexBufferDescriptor->Size;
			bufferInfo.usage		= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			bufferInfo.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

			VK_ThrowIfFailed(vkCreateBuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &bufferInfo, nullptr, &m_VertexBufferObj));
			
			// Associate buffer with memory
			vkBindBufferMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_VertexBufferObj, NEON_CAST(VKMemoryPool*, _vertexBufferDescriptor->VMemoryPool)->m_memoryObj, 0);

			void* data = Map();
			
			memcpy(data, _vertexBufferDescriptor->Vertices, _vertexBufferDescriptor->Size);
			
			Unmap();

#if defined(NEON_DEBUG)
			VkDebugUtilsObjectNameInfoEXT objectNameInfo{};
			objectNameInfo.sType		= VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
			objectNameInfo.objectType	= VK_OBJECT_TYPE_BUFFER;
			objectNameInfo.objectHandle = (uint64_t)m_VertexBufferObj;
			objectNameInfo.pObjectName	= _vertexBufferDescriptor->Name.data();
	
			// SetName
			auto func = VKGraphicsContext::GetInstance()->GetDebugNameUtils();
			func(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &objectNameInfo);
#endif
		}

		VKVertexBuffer::~VKVertexBuffer()
		{
			vkDestroyBuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_VertexBufferObj, nullptr);
		}

		void* VKVertexBuffer::Map() const
		{
			void* data;
			vkMapMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), NEON_CAST(VKMemoryPool*, m_MemoryPool)->m_memoryObj, 0, m_Size, 0, &data);
			return data;
		}

		void VKVertexBuffer::Unmap() const
		{
			vkUnmapMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), NEON_CAST(VKMemoryPool*, m_MemoryPool)->m_memoryObj);
		}
	}
}