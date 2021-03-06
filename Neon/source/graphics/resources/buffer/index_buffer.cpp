#include "./graphics/resources/buffer/index_buffer.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/resources/buffer/dx11_index_buffer.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/resources/buffer/dx12_index_buffer.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/resources/buffer/vk_index_buffer.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		IndexBuffer* IndexBuffer::Create(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKIndexBuffer(_commandBuffer, _indexBufferDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11IndexBuffer(_commandBuffer, _indexBufferDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12IndexBuffer(_commandBuffer, _indexBufferDescriptor);

			printf("[ERROR] IndexBuffer::Create() No valid api abstraction for found for object: IndexBuffer");
			return nullptr;
		}

		IndexBuffer::IndexBuffer(const IndexBufferDescriptor* _indexBufferDescriptor)
			: m_Size(_indexBufferDescriptor->Size)
			, m_IndexCount(_indexBufferDescriptor->IndexCount)
			, m_BufferUsage(_indexBufferDescriptor->Usage)
			, m_BufferFormat(_indexBufferDescriptor->Format)
			, m_MemoryPool(_indexBufferDescriptor->IMemoryPool)
#if defined(NEON_DEBUG)
			, m_Name(_indexBufferDescriptor->Name)
#endif
		{ }
	}
}