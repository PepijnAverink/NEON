#pragma once
#include "./graphics/resources/buffer/vertex_buffer_descriptor.h"
#include "./graphics/graphics_defines.h"

namespace Neon
{
	namespace Graphics
	{
		class CommandBuffer;
		class VertexBuffer
		{
		public:
			static VertexBuffer* Create(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor);
			virtual ~VertexBuffer() { }

			virtual void* Map()   const = 0;
			virtual void  Unmap() const = 0;

			inline const uint32_t GetVertexCount() const { return m_VertexCount; }

		protected:
			VertexBuffer(const VertexBufferDescriptor* _vertexBufferDescriptor);

#if defined(NEON_DEBUG)
			std::string		   m_Name;
#endif
			uint32_t		   m_VertexStride;
			uint32_t		   m_VertexCount;
			BufferUsage		   m_BufferUsage;

			MemoryPool*		   m_MemoryPool;
			uint32_t		   m_Size;
			uint32_t		   m_Offset;
		};
	}
}