#include "./graphics/resources/buffer/compute_buffer.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/resources/buffer/dx11_compute_buffer.h"
#endif

//#if defined (NEON_SUPPORT_DIRECTX12)	
//#include "./graphics/api/directx12/resources/buffer/dx12_index_buffer.h"
//#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/resources/buffer/vk_index_buffer.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		ComputeBuffer* ComputeBuffer::Create(const ComputeBufferDescriptor* _computeBufferDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

		//	// Vulkan
		//	if (api == GraphicsAPI::VULKAN)
		//		return new VKIndexBuffer(_commandBuffer, _indexBufferDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11ComputeBuffer(_computeBufferDescriptor);

		//	// DirectX12
		//	if (api == GraphicsAPI::DIRECTX12)
		//		return new DX12IndexBuffer(_commandBuffer, _indexBufferDescriptor);

			printf("[ERROR] ComputeBuffer::Create() No valid api abstraction for found for object: ComputeBuffer");
			return nullptr;
		}

		ComputeBuffer::ComputeBuffer(const ComputeBufferDescriptor* _computeBufferDescriptor)
			: m_Size(_computeBufferDescriptor->Size)
		{ }
	}
}