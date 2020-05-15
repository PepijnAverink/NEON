#include "./graphics/api/vulkan/objects/swapchain/vk_swapchain.h"
#include "./graphics/api/vulkan/objects/swapchain/vk_graphics_surface.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./core/window/window.h"
#include "./utilities/casting/casting_helper.h"

#include <windows.h>

namespace Neon
{
	namespace Graphics
	{
		VKSwapchain::VKSwapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor)
			: Swapchain(_swapchainDescriptor)
		{
			// Get the surface
			auto surface = NEON_CAST(VKGraphicsSurface*, _swapchainDescriptor->Surface)->m_WindowSurfaceObj;

			// Find surface capabilities
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			VK_ThrowIfFailed(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), surface, &surfaceCapabilities));

			// Find supported surface formats
			uint32_t formatCount;
			VK_ThrowIfFailed(vkGetPhysicalDeviceSurfaceFormatsKHR(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), surface, &formatCount, nullptr));

			if (formatCount == 0)
				VK_ThrowIfFailed(VK_NOT_READY);

			std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
			VK_ThrowIfFailed(vkGetPhysicalDeviceSurfaceFormatsKHR(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), surface, &formatCount, surfaceFormats.data()));


			// Find supported present modes
			uint32_t presentModeCount;
			VK_ThrowIfFailed(vkGetPhysicalDeviceSurfacePresentModesKHR(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), surface, &presentModeCount, nullptr));

			if (presentModeCount == 0)
				VK_ThrowIfFailed(VK_NOT_READY);

			std::vector<VkPresentModeKHR> presentModes(presentModeCount);
			VK_ThrowIfFailed(vkGetPhysicalDeviceSurfacePresentModesKHR(VKGraphicsContext::GetInstance()->GetPhysicalDevice(), surface, &presentModeCount, presentModes.data()));

			// Refit ImageCount
			if (_swapchainDescriptor->BackBufferCount < surfaceCapabilities.minImageCount)
				m_BackBufferCount = surfaceCapabilities.minImageCount;
			if (_swapchainDescriptor->BackBufferCount > surfaceCapabilities.maxImageCount)
				m_BackBufferCount = surfaceCapabilities.maxImageCount;


			VkFormat swapChainImageFormat = VK_FORMAT_R8G8B8A8_UNORM;

			// Select swap chain size
			VkExtent2D swapChainExtent = { _swapchainDescriptor->Width, _swapchainDescriptor->Height };

			// Check if swap chain supports being the destination of an image transfer
			// Note: AMD driver bug, though it would be nice to implement a workaround that doesn't use transfering
			if (!(surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT))
				VK_ThrowIfFailed(VK_NOT_READY);

			// Determine transformation to use (preferring no transform)
			VkSurfaceTransformFlagBitsKHR surfaceTransform;
			if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
				surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else {
				surfaceTransform = surfaceCapabilities.currentTransform;
			}

			// Choose presentation mode (preferring MAILBOX ~= triple buffering)
			VkPresentModeKHR presentMode = VK_PRESENT_MODE_MAILBOX_KHR;

			// Finally, create the swap chain
			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface					= surface;
			createInfo.minImageCount			= m_BackBufferCount;
			createInfo.imageFormat				= swapChainImageFormat;
			createInfo.imageColorSpace			= VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			createInfo.imageExtent				= swapChainExtent;
			createInfo.imageArrayLayers			= 1;
			createInfo.imageUsage				= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			createInfo.imageSharingMode			= VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount	= 0;
			createInfo.pQueueFamilyIndices		= nullptr;
			createInfo.preTransform				= surfaceTransform;
			createInfo.compositeAlpha			= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode				= presentMode;
			createInfo.clipped					= VK_TRUE;
			createInfo.oldSwapchain				= VK_NULL_HANDLE;

			VK_ThrowIfFailed(vkCreateSwapchainKHR(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &createInfo, nullptr, &m_SwapchainObj));
		}

		VKSwapchain::~VKSwapchain()
		{

		}

		FramebufferAttachment* VKSwapchain::GetFramebufferAttachment(const int _i) const
		{
			return nullptr;
		}

		void VKSwapchain::Resize(const int _width, const int _height)
		{

		}

		int VKSwapchain::AquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence)
		{
			return 0;
		}

		void VKSwapchain::Present(CommandQueue* _commandQueue, const bool _vsync) const
		{

		}
	}
}