#include "./graphics/resources/texture/texture2D.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/resources/texture/dx11_texture2D.h"
#endif

//#if defined (NEON_SUPPORT_DIRECTX12)	
//#include "./graphics/api/directx12/resources/buffer/dx12_vertex_buffer.h"
//#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/resources/buffer/vk_vertex_buffer.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		Texture2D* Texture2D::Load(const std::string& _filename)
		{
			return nullptr;
		}

		Texture2D* Texture2D::Create(CommandBuffer* _commandBuffer, const Texture2DDescriptor* _texture2DDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

		//	// Vulkan
		//	if (api == GraphicsAPI::VULKAN)
		//		return new VKVertexBuffer(_commandBuffer, _texture2DDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11Texture2D(_commandBuffer, _texture2DDescriptor);

		//	// DirectX12
		//	if (api == GraphicsAPI::DIRECTX12)
		//		return new DX12VertexBuffer(_commandBuffer, _texture2DDescriptor);

			printf("[ERROR] VertexBuffer::Create() No valid api abstraction for found for object: VertexBuffer");
			return nullptr;
		}

		Texture2D::Texture2D(const Texture2DDescriptor* _texture2DDectiptor)
			: m_Width(_texture2DDectiptor->Width)
			, m_Height(_texture2DDectiptor->Height)
		{ }
	}
}