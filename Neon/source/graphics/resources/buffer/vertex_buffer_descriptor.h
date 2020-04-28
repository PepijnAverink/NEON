#pragma once
#include "./graphics/resources/buffer/buffer_usage.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class MemoryPool;
		class VertexBufferDescriptor
		{
		public:
			VertexBufferDescriptor() = default;

			std::string Name;
			void*		Vertices;
			uint32_t	VertexCount;

			BufferUsage Usage;
			MemoryPool* VMemoryPool;
			uint32_t	Size;
			uint32_t    Offset;
		};
	}
}