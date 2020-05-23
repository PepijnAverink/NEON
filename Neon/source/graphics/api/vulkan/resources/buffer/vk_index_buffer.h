#pragma once
#include "./graphics/resources/buffer/index_buffer.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKIndexBuffer : public IndexBuffer
		{
		public:
			VKIndexBuffer(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor);
			virtual ~VKIndexBuffer();

			virtual void* Map()   const override;
			virtual void  Unmap() const override;

		private:
			friend class VKCommandBuffer;
			friend class VKGraphicsContext; // TODO:: remove me
			VkBuffer m_IndexBufferObj;
		};
	}
}