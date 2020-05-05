#include "./graphics/objects/renderpass/renderpass.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/renderpass/dx12_renderpass.h"
#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		Renderpass* Renderpass::Create(const RenderpassDescriptor* _renderpassDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			//if (api == GraphicsAPI::VULKAN)
			//	return new VKCommandBuffer(_renderpassDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Renderpass(_renderpassDescriptor);

			printf("[ERROR] Renderpass::Create() No valid api abstraction for found for object: Renderpass");
			return nullptr;
		}

		Renderpass::Renderpass(const RenderpassDescriptor* _renderpassDescriptor)
		{

		}
	}
}