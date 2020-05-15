#pragma once
#include "./graphics/resources/buffer/index_buffer_descriptor.h"
#include "./graphics/graphics_defines.h"

namespace Neon
{
	namespace Graphics
	{
		class CommandBuffer;
		class IndexBuffer
		{
		public:
			static IndexBuffer* Create(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor);
			virtual ~IndexBuffer() {}

			virtual void* Map()   const = 0;
			virtual void  Unmap() const = 0;

			inline const uint32_t GetIndexCount() const { return m_IndexCount; }

		protected:
			IndexBuffer(const IndexBufferDescriptor* _indexBufferDescriptor);

#if defined(NEON_DEBUG)
			std::string		   m_Name;
#endif
			MemoryPool*		   m_MemoryPool;

			uint32_t		   m_Size;
			uint32_t		   m_IndexCount;

			BufferUsage		   m_BufferUsage;
			BufferFormat	   m_BufferFormat;
		};
	}
}