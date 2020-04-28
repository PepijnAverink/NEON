#include "./graphics/api/vulkan/resources/memory/vk_memory_pool.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"

#include "./graphics/api/vulkan/vk_error.h"

#define VULKAN_MINIMUM_MEMORY_POOL_SIZE 256
#define VULKAN_MEMORY_ALIGNMENT_STORAGE 32
#define VULKAN_MEMORY_ALIGNMENT_UNIFORM 256

namespace Neon
{
	namespace Graphics
	{
		VkPhysicalDeviceLimits;
		VKMemoryPool::VKMemoryPool(const MemoryPoolDescriptor* _memoryPoolDescriptor)
			: MemoryPool(_memoryPoolDescriptor)
		{
		//	// Setup a dummy buffer
		//	VkBufferCreateInfo bufferInfo{};
		//	bufferInfo.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		//	bufferInfo.size			= _memoryPoolDescriptor->Size;
		//	bufferInfo.usage		= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		//	bufferInfo.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;
		//
		//	VkBuffer dummyBuffer;
		//	if (vkCreateBuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &bufferInfo, nullptr, &dummyBuffer) != VK_SUCCESS) {
		//		throw std::runtime_error("failed to create vertex buffer!");
		//	}
		//
		//	VkMemoryRequirements memRequirements;
		//	vkGetBufferMemoryRequirements(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), dummyBuffer, &memRequirements);
		//
		//	vkDestroyBuffer(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), dummyBuffer, nullptr);
			uint32_t s = _memoryPoolDescriptor->Size < VULKAN_MINIMUM_MEMORY_POOL_SIZE ? VULKAN_MINIMUM_MEMORY_POOL_SIZE : _memoryPoolDescriptor->Size;
			s = Align(s, VULKAN_MEMORY_ALIGNMENT_STORAGE);

			VkMemoryAllocateInfo allocInfo{};
			allocInfo.sType			  = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize  = s;
			allocInfo.memoryTypeIndex = FindMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

			// Allocate memory
			VK_ThrowIfFailed(vkAllocateMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &allocInfo, nullptr, &m_memoryObj));
		}

		VKMemoryPool::~VKMemoryPool()
		{
			vkFreeMemory(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_memoryObj, nullptr);
		}

		uint32_t VKMemoryPool::FindMemoryType(VkMemoryPropertyFlags properties)
		{
			// Memory properties
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
				if ((memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
					return i;
				}
			}

			printf("[ERROR] VKMemoryPool() - Specified type of memory could not be found.");
			return -1;
		}
	}
}