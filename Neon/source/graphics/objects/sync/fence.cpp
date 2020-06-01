#include "./graphics/objects/sync/fence.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/objects/sync/dx11_fence.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/sync/dx12_fence.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/sync/vk_fence.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		Fence* Fence::Create(const FenceDescriptor* _fenceDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKFence(_fenceDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11Fence(_fenceDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Fence(_fenceDescriptor);

			printf("[ERROR] CommandAllocator::Create() No valid api abstraction for found for object: Fence");
			return nullptr;
		}

		Fence::Fence(const FenceDescriptor* _fenceDescriptor)
			: m_TimeOut(_fenceDescriptor->TimeOut)
		{ }
	}
}