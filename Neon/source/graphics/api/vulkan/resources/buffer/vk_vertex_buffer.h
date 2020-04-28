#pragma once
#include "./graphics/resources/buffer/vertex_buffer.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKVertexBuffer : public VertexBuffer
		{
		public:
			VKVertexBuffer(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor);
			virtual ~VKVertexBuffer();

			virtual void* Map()   const override;
			virtual void  Unmap() const override;

		private:
			friend class VKGraphicsContext; // TODO:: remove me
			VkBuffer m_VertexBufferObj;
		};
	}
}