#include "./graphics/objects/swapchain/swapchain.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/objects/swapchain/dx11_swapchain.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/swapchain/dx12_swapchain.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/swapchain/vk_swapchain.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		Swapchain* Swapchain::Create(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKSwapchain(_commandQueue, _swapchainDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11Swapchain(_commandQueue, _swapchainDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Swapchain(_commandQueue, _swapchainDescriptor);

			printf("[ERROR] Swapchain::Create() No valid api abstraction for found for object: Swapchain");
			return nullptr;
		}

		Swapchain::Swapchain(const SwapchainDescriptor* _swapchainDescriptor)
			: m_BackBufferCount(_swapchainDescriptor->BackBufferCount)
			, m_Width(_swapchainDescriptor->Width)
			, m_Height(_swapchainDescriptor->Height)
		{ }
	}
}