#pragma once
#include "./graphics/resources/memory/memory_pool_descriptor.h"
#include "./graphics/graphics_defines.h"

namespace Neon
{
	namespace Graphics
	{
		class MemoryPool
		{
		public:
			static MemoryPool* Create(const MemoryPoolDescriptor* _memoryPoolDescriptor);
			virtual ~MemoryPool() { }

		protected:
			MemoryPool(const MemoryPoolDescriptor* _memoryPoolDescriptor);
			uint32_t Align(uint32_t size, uint32_t alignment);


#if defined(NEON_DEBUG)
			std::string m_Name;
#endif
			MemoryAccess m_MemoryAccess;
			uint32_t	 m_Size;
		};
	}
}