#include "./graphics/objects/command_generic/scissor.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/command_generic/dx12_scissor.h"
#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
//#endif

#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		Scissor* Scissor::Create(const float _bottomX, const float _bottomY, const float _width, const float _height)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

		//	// Vulkan
		//	if (api == GraphicsAPI::VULKAN)
		//		return new VKCommandBuffer(_commandBufferDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Scissor(_bottomX, _bottomY, _width, _height);

			printf("[ERROR] Scissor::Create() No valid api abstraction for found for object: Scissor");
			return nullptr;
		}

		Scissor::Scissor(const float _bottomX, const float _bottomY, const float _width, const float _height)
			: m_BottomX(_bottomX)
			, m_BottomY(_bottomY)
			, m_Width(_width)
			, m_Height(_height)
		{ }
	}
}