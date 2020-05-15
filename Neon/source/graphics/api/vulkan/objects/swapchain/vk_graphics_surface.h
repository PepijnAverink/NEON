#pragma once
#include "./graphics/objects/swapchain/graphics_surface.h"

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKGraphicsSurface : public GraphicsSurface
		{
		public:
			VKGraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor);

		private:
			friend class VKSwapchain;
			friend class VKGraphicsContext; // Remove me
			VkSurfaceKHR   m_WindowSurfaceObj;
		};
	}
}