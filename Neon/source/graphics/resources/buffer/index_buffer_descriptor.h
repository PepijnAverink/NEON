#pragma once
#include "./graphics/resources/buffer/buffer_usage.h"
#include "./graphics/resources/buffer/buffer_format.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class MemoryPool;
		class IndexBufferDescriptor
		{
		public:
			IndexBufferDescriptor() = default;

			std::string  Name;
			void*		 Indices;
			uint32_t	 IndexCount;

			BufferUsage	 Usage;
			BufferFormat Format;

			MemoryPool*  IMemoryPool;
			uint32_t	 Size;
			uint32_t     Offset;
		};
	}
}