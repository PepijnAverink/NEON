#include "./graphics/objects/swapchain/graphics_surface.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/objects/swapchain/dx11_graphics_surface.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/swapchain/dx12_graphics_surface.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/swapchain/vk_graphics_surface.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		GraphicsSurface* GraphicsSurface::Create(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKGraphicsSurface(_graphicsSurfaceDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11GraphicsSurface(_graphicsSurfaceDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12GraphicsSurface(_graphicsSurfaceDescriptor);

			printf("[ERROR] GraphicsSurface::Create() No valid api abstraction for found for object: GraphicsSurface");
			return nullptr;
		}

		GraphicsSurface::GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor)
		{ }
	}
}