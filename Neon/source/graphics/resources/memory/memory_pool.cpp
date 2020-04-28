#include "./graphics/resources/memory/memory_pool.h"

#include "./graphics/graphics_driver.h"

//#if defined (NEON_SUPPORT_DIRECTX12)	
//#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"
//#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/resources/memory/vk_memory_pool.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		MemoryPool* MemoryPool::Create(const MemoryPoolDescriptor* _memoryPoolDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKMemoryPool(_memoryPoolDescriptor);

			// DirectX12
		//	if (api == GraphicsAPI::DIRECTX12)
		//		return new DX12CommandBuffer(_memoryPoolDescriptor);

			printf("[ERROR] MemoryPool::Create() No valid api abstraction for found for object: MemoryPool");
			return nullptr;
		}

		MemoryPool::MemoryPool(const MemoryPoolDescriptor* _memoryPoolDescriptor)
			: m_MemoryAccess(_memoryPoolDescriptor->Access)
			, m_Size(_memoryPoolDescriptor->Size)
#if defined(NEON_DEBUG)
			, m_Name(_memoryPoolDescriptor->Name)
#endif
		{ }

		uint32_t MemoryPool::Align(uint32_t size, uint32_t alignment)
		{
			size_t temp = (alignment - (size % alignment)) % alignment;
			return temp + size;
		}
	}
}