#pragma once
#include "./graphics/resources/memory/memory_access.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class MemoryPoolDescriptor
		{
		public:
			MemoryPoolDescriptor() = default;

			std::string Name;
			uint32_t    Size;
			MemoryAccess  Access;
		};
	}
}