#include "./graphics/api/vulkan/objects/swapchain/vk_graphics_surface.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./core/window/window.h"

namespace Neon
{
	namespace Graphics
	{
		VKGraphicsSurface::VKGraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor)
			: GraphicsSurface(_graphicsSurfaceDescriptor)
		{
			// Setup the surface
			VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
			surfaceCreateInfo.hwnd      = (HWND)_graphicsSurfaceDescriptor->Window->GetNativeWindowHandle();

			// Create WindowSurface
			VK_ThrowIfFailed(vkCreateWin32SurfaceKHR(VKGraphicsContext::GetInstance()->GetGraphicsInstance(), &surfaceCreateInfo, NULL, &m_WindowSurfaceObj));

			// Validate
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), &queueFamilyCount, nullptr);

			if (queueFamilyCount == 0) {
				printf("[ERROR] VKGraphicsSurface::VKGraphicsSurface() - No queue families found on the current device.\n");
				return;
			}

			// Get queue properties
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), &queueFamilyCount, queueFamilies.data());

			for (uint32_t i = 0; i < queueFamilyCount; i++) {
				VkBool32 presentSupport = true;
				vkGetPhysicalDeviceSurfaceSupportKHR(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), i, m_WindowSurfaceObj, &presentSupport);

				if (presentSupport == VK_TRUE)
					return;
			}

		}
		VKGraphicsSurface::~VKGraphicsSurface()
		{
			vkDestroySurfaceKHR(VKGraphicsContext::GetInstance()->GetGraphicsInstance(), m_WindowSurfaceObj, nullptr);
		}
	}
}