#pragma once
#include "./graphics/resources/memory/memory_pool.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKMemoryPool : public MemoryPool
		{
		public:
			VKMemoryPool(const MemoryPoolDescriptor* _memoryPoolDescriptor);
			virtual ~VKMemoryPool();

		private:
			uint32_t FindMemoryType(VkMemoryPropertyFlags properties);

			friend class VKVertexBuffer;
			VkDeviceMemory m_memoryObj;
		};
	}
}