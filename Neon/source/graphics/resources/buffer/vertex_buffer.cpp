#include "./graphics/resources/buffer/vertex_buffer.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/resources/buffer/dx12_vertex_buffer.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/resources/buffer/vk_vertex_buffer.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		VertexBuffer* VertexBuffer::Create(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKVertexBuffer(_commandBuffer, _vertexBufferDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12VertexBuffer(_commandBuffer, _vertexBufferDescriptor);

			printf("[ERROR] VertexBuffer::Create() No valid api abstraction for found for object: VertexBuffer");
			return nullptr;
		}

		VertexBuffer::VertexBuffer(const VertexBufferDescriptor* _vertexBufferDescriptor)
			: m_VertexCount(_vertexBufferDescriptor->VertexCount)
			, m_BufferUsage(_vertexBufferDescriptor->Usage)
			, m_MemoryPool(_vertexBufferDescriptor->VMemoryPool)
			, m_Size(_vertexBufferDescriptor->Size)
			, m_Offset(_vertexBufferDescriptor->Offset)
#if defined(NEON_DEBUG)
			, m_Name(_vertexBufferDescriptor->Name)
#endif
		{ }
	}
}