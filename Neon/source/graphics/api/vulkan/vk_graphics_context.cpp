#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./core/window/window.h"

// Some defines
#include "./core/core_defines.h"
#include "./graphics/api/vulkan/vk_error.h"

#include <assert.h>
#include <vector>
#include <iostream>

#include <fstream>

namespace Neon
{
	namespace Graphics
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		HWND hwnd;
		VkDebugUtilsMessengerEXT debugMessenger;

		VkInstance instance;
		VkSurfaceKHR windowSurface;
	

		VkFormat swapChainImageFormat;
		VkQueue graphicsQueue;
	//	VkQueue presentQueue;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderingFinishedSemaphore;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;

		uint32_t graphicsQueueFamily;
		uint32_t presentQueueFamily;

		VkRenderPass	 m_RenderPass;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline		 m_GraphicsPipeline;

		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkPresentModeKHR choosePresentMode(const std::vector<VkPresentModeKHR> presentModes) {
			for (const auto& presentMode : presentModes) {
				if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
					return presentMode;
				}
			}

			// If mailbox is unavailable, fall back to FIFO (guaranteed to be available)
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkSurfaceFormatKHR chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
			// We can either choose any format
			if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
				return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
			}

			// Or go with the standard format - if available
			for (const auto& availableSurfaceFormat : availableFormats) {
				if (availableSurfaceFormat.format == VK_FORMAT_R8G8B8A8_UNORM) {
					return availableSurfaceFormat;
				}
			}

			// Or fall back to the first available one
			return availableFormats[0];
		}

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& surfaceCapabilities) {
			if (surfaceCapabilities.currentExtent.width == -1) {
				VkExtent2D swapChainExtent = {};

				swapChainExtent.width = min(max(640, surfaceCapabilities.minImageExtent.width), surfaceCapabilities.maxImageExtent.width);
				swapChainExtent.height = min(max(480, surfaceCapabilities.minImageExtent.height), surfaceCapabilities.maxImageExtent.height);

				return swapChainExtent;
			}
			else {
				return surfaceCapabilities.currentExtent;
			}
		}

		void VKGraphicsContext::findQueueFamilies() {
			// Check queue families
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

			if (queueFamilyCount == 0) {
				std::cout << "physical device has no queue families!" << std::endl;
				exit(1);
			}

			// Find queue family with graphics support
			// Note: is a transfer queue necessary to copy vertices to the gpu or can a graphics queue handle that?
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

			std::cout << "physical device has " << queueFamilyCount << " queue families" << std::endl;

			bool foundGraphicsQueueFamily = false;
			bool foundPresentQueueFamily = false;

			for (uint32_t i = 0; i < queueFamilyCount; i++) {
				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, windowSurface, &presentSupport);

				if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					graphicsQueueFamily = i;
					foundGraphicsQueueFamily = true;

					if (presentSupport) {
						presentQueueFamily = i;
						foundPresentQueueFamily = true;
						break;
					}
				}

				if (!foundPresentQueueFamily && presentSupport) {
					presentQueueFamily = i;
					foundPresentQueueFamily = true;
				}
			}

			if (foundGraphicsQueueFamily) {
				std::cout << "queue family #" << graphicsQueueFamily << " supports graphics" << std::endl;

				if (foundPresentQueueFamily) {
					std::cout << "queue family #" << presentQueueFamily << " supports presentation" << std::endl;
				}
				else {
					std::cerr << "could not find a valid queue family with present support" << std::endl;
					exit(1);
				}
			}
			else {
				std::cerr << "could not find a valid queue family with graphics support" << std::endl;
				exit(1);
			}
		}

		VKGraphicsContext::VKGraphicsContext(Core::Window* _window)
			: GraphicsContext(_window)
		{
			// Remove me
			hwnd = (HWND)_window->GetNativeWindowHandle();

			CreateInstance();


			SetDebugUtilsObjectName = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT");

			CreateSurface();
			FindPhysicalDevice();
			CheckSwapchainSupport();

			findQueueFamilies();
			m_GraphicsQueueFamilyID = graphicsQueueFamily;

			CreateLogicalDevice();
			CreateSemaphores();

			CreateSwapchain();

			createRenderPass();
			createGraphicsPipeline();

			CreateCommandQueues();
		}

		VKGraphicsContext::~VKGraphicsContext()
		{
			
		}

		bool VKGraphicsContext::Initialize()
		{

			return false;
		}

		bool VKGraphicsContext::Terminate()
		{
			for (unsigned int i = 0; i < commandBuffers.size(); ++i)
				delete commandBuffers[i];

			delete commandPool;
			delete commandQueue;

			delete submitFence;
			delete acuireFence;

#if defined(NEON_DEBUG)
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			func(instance, debugMessenger, nullptr);
#endif

			vkDestroyInstance(instance, nullptr);
			return false;
		}

		void VKGraphicsContext::Present()
		{
			// Acquire image
			uint32_t imageIndex;
			VkResult res = vkAcquireNextImageKHR(m_Device, swapChain, UINT64_MAX,VK_NULL_HANDLE, NEON_CAST(VKFence*, acuireFence)->m_FenceObj, &imageIndex);
		//	VkResult res = vkAcquireNextImageKHR(m_Device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

			acuireFence->WaitForFence();
			acuireFence->Reset();

			if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
				std::cerr << "failed to acquire image" << std::endl;
			//	exit(1);
			}

		//	std::cout << "acquired image" << std::endl;

		//	// Wait for image to be available and draw
		//	VkSubmitInfo submitInfo = {};
		//	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		//
		//	submitInfo.waitSemaphoreCount = 1;
		//	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
		//
		//	submitInfo.signalSemaphoreCount = 1;
		//	submitInfo.pSignalSemaphores = &renderingFinishedSemaphore;
		//
		//	// This is the stage where the queue should wait on the semaphore (it doesn't have to wait with drawing, for example)
		//	VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_TRANSFER_BIT;
		//	submitInfo.pWaitDstStageMask = &waitDstStageMask;
		//
		//	submitInfo.commandBufferCount = 1;
		//	submitInfo.pCommandBuffers = &NEON_CAST(VKCommandBuffer*, commandBuffers[imageIndex])->m_CommandBufferObj;
		//
		//	if (vkQueueSubmit(NEON_CAST(VKCommandQueue*, commandQueue)->m_CommandQueueObj, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
		//		std::cerr << "failed to submit draw command buffer" << std::endl;
		//		exit(1);
		//	}
			commandQueue->ExecuteCommandBuffer(commandBuffers[imageIndex], submitFence);

			submitFence->WaitForFence();
			submitFence->Reset();

		//	std::cout << "submitted draw command buffer" << std::endl;

			// Present drawn image
			// Note: semaphore here is not strictly necessary, because commands are processed in submission order within a single queue
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		//	presentInfo.waitSemaphoreCount = 1;
		//	presentInfo.pWaitSemaphores = &renderingFinishedSemaphore;
			presentInfo.waitSemaphoreCount = 0;
			presentInfo.pWaitSemaphores = VK_NULL_HANDLE;
			

			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &swapChain;
			presentInfo.pImageIndices = &imageIndex;

			res = vkQueuePresentKHR(NEON_CAST(VKCommandQueue*, commandQueue)->m_CommandQueueObj, &presentInfo);

			if (res != VK_SUCCESS) {
				std::cerr << "failed to submit present command buffer" << std::endl;
			//	exit(1);
			}

		//	std::cout << "submitted presentation command buffer" << std::endl;
		}

		void VKGraphicsContext::CreateInstance()
		{
			// Application info
			VkApplicationInfo appInfo = {};
			appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName	= "VulkanClear";
			appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName			= "ClearScreenEngine";
			appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion			= VK_API_VERSION_1_0;

			// Check for extensions
		//	uint32_t extensionCount = 0;
		//	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		//
		//	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		//	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
		//
		//	printf("supported extensions:\n");
		//	for (const auto& extension : availableExtensions) 
		//		printf("\t-%s\n", extension.extensionName);

			// Define layers
			std::vector<const char*> layers;
#if defined(NEON_DEBUG)
			layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

			// Check layer support
			assert(CheckLayersSupport(layers));

			// Define extensions
			std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
#if defined(NEON_DEBUG)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

			// Check extesion support
			assert(CheckExtensionsSupport(extensions));

			// Creation info
			VkInstanceCreateInfo createInfo		= {};
			createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo			= &appInfo;
			createInfo.enabledLayerCount		= static_cast<uint32_t>(layers.size());
			createInfo.ppEnabledLayerNames		= layers.data();
			createInfo.enabledExtensionCount	= static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames	= extensions.data();
			createInfo.pNext = nullptr;

#if defined(NEON_DEBUG)
			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
			debugCreateInfo.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugCreateInfo.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugCreateInfo.pfnUserCallback = debugCallback;

			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
#endif

			// Initialize Vulkan instance
			VK_ThrowIfFailed(vkCreateInstance(&createInfo, nullptr, &instance));

#if defined(NEON_DEBUG)
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			func(instance, &debugCreateInfo, nullptr, &debugMessenger);
#endif
		}

		void VKGraphicsContext::CreateSurface()
		{
			// Setup the surface
			VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
			surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
			surfaceCreateInfo.hwnd = (HWND)hwnd;

			VK_ThrowIfFailed(vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &windowSurface));
		}

		// TODO:: find best device
		void VKGraphicsContext::FindPhysicalDevice()
		{
			// Try to find 1 Vulkan supported device
			uint32_t deviceCount = 1;
			VkResult res = vkEnumeratePhysicalDevices(instance, &deviceCount, &m_PhysicalDevice);
			if (res != VK_SUCCESS && res != VK_INCOMPLETE) {
				std::cerr << "enumerating physical devices failed!" << std::endl;
				exit(1);
			}

			if (deviceCount == 0) {
				std::cerr << "no physical devices that support vulkan!" << std::endl;
				exit(1);
			}

			std::cout << "physical device with vulkan support found" << std::endl;

			// Check device features
			// Note: will apiVersion >= appInfo.apiVersion? Probably yes, but spec is unclear.
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);
			vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &deviceFeatures);

			uint32_t supportedVersion[] = {
				VK_VERSION_MAJOR(deviceProperties.apiVersion),
				VK_VERSION_MINOR(deviceProperties.apiVersion),
				VK_VERSION_PATCH(deviceProperties.apiVersion)
			};

			std::cout << "physical device supports version " << supportedVersion[0] << "." << supportedVersion[1] << "." << supportedVersion[2] << std::endl;
		}

		bool VKGraphicsContext::CheckSwapchainSupport()
		{
			uint32_t extensionCount = 0;
			vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, nullptr);

			if (extensionCount == 0) {
				std::cerr << "physical device doesn't support any extensions" << std::endl;
				exit(1);
			}

			std::vector<VkExtensionProperties> deviceExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(m_PhysicalDevice, nullptr, &extensionCount, deviceExtensions.data());

			for (const auto& extension : deviceExtensions) {
				if (strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
					std::cout << "physical device supports swap chains" << std::endl;
					return true;
				}
			}

			std::cerr << "physical device doesn't support swap chains" << std::endl;
			exit(1);
			return false;
		}

		void VKGraphicsContext::CreateLogicalDevice()
		{
			// Greate one graphics queue and optionally a separate presentation queue
			float queuePriority = 1.0f;

			VkDeviceQueueCreateInfo queueCreateInfo[2] = {};

			queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[0].queueFamilyIndex = graphicsQueueFamily;
			queueCreateInfo[0].queueCount = 1;
			queueCreateInfo[0].pQueuePriorities = &queuePriority;

			queueCreateInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo[1].queueFamilyIndex = presentQueueFamily;
			queueCreateInfo[1].queueCount = 1;
			queueCreateInfo[1].pQueuePriorities = &queuePriority;

			// Create logical device from physical device
			// Note: there are separate instance and device extensions!
			VkDeviceCreateInfo deviceCreateInfo = {};
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;

			if (graphicsQueueFamily == presentQueueFamily) {
				deviceCreateInfo.queueCreateInfoCount = 1;
			}
			else {
				deviceCreateInfo.queueCreateInfoCount = 2;
			}

			const char* deviceExtensions = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
			deviceCreateInfo.enabledExtensionCount = 1;
			deviceCreateInfo.ppEnabledExtensionNames = &deviceExtensions;

			if (vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device) != VK_SUCCESS) {
				std::cerr << "failed to create logical device" << std::endl;
				exit(1);
			}

			std::cout << "created logical device" << std::endl;

			// Get graphics and presentation queues (which may be the same)
			vkGetDeviceQueue(m_Device, graphicsQueueFamily, 0, &graphicsQueue);
		//	vkGetDeviceQueue(m_Device, presentQueueFamily, 0, &presentQueue);

			std::cout << "acquired graphics and presentation queues" << std::endl;
		}

		void VKGraphicsContext::CreateSemaphores()
		{
			VkSemaphoreCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(m_Device, &createInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
				vkCreateSemaphore(m_Device, &createInfo, nullptr, &renderingFinishedSemaphore) != VK_SUCCESS) {
				std::cerr << "failed to create semaphores" << std::endl;
				exit(1);
			}
			else {
				std::cout << "created semaphores" << std::endl;
			}
		}

		void VKGraphicsContext::CreateSwapchain()
		{
			// Find surface capabilities
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_PhysicalDevice, windowSurface, &surfaceCapabilities) != VK_SUCCESS) {
				std::cerr << "failed to acquire presentation surface capabilities" << std::endl;
				exit(1);
			}

			// Find supported surface formats
			uint32_t formatCount;
			if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, windowSurface, &formatCount, nullptr) != VK_SUCCESS || formatCount == 0) {
				std::cerr << "failed to get number of supported surface formats" << std::endl;
				exit(1);
			}

			std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
			if (vkGetPhysicalDeviceSurfaceFormatsKHR(m_PhysicalDevice, windowSurface, &formatCount, surfaceFormats.data()) != VK_SUCCESS) {
				std::cerr << "failed to get supported surface formats" << std::endl;
				exit(1);
			}

			// Find supported present modes
			uint32_t presentModeCount;
			if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, windowSurface, &presentModeCount, nullptr) != VK_SUCCESS || presentModeCount == 0) {
				std::cerr << "failed to get number of supported presentation modes" << std::endl;
				exit(1);
			}

			std::vector<VkPresentModeKHR> presentModes(presentModeCount);
			if (vkGetPhysicalDeviceSurfacePresentModesKHR(m_PhysicalDevice, windowSurface, &presentModeCount, presentModes.data()) != VK_SUCCESS) {
				std::cerr << "failed to get supported presentation modes" << std::endl;
				exit(1);
			}

			// Determine number of images for swap chain
			uint32_t imageCount = surfaceCapabilities.minImageCount + 1;
			if (surfaceCapabilities.maxImageCount != 0 && imageCount > surfaceCapabilities.maxImageCount) {
				imageCount = surfaceCapabilities.maxImageCount;
			}

			std::cout << "using " << imageCount << " images for swap chain" << std::endl;

			// Select a surface format
			// Select a surface format
			VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(surfaceFormats);
			swapChainImageFormat = surfaceFormat.format;

			// Select swap chain size
			VkExtent2D swapChainExtent = chooseSwapExtent(surfaceCapabilities);

			// Check if swap chain supports being the destination of an image transfer
			// Note: AMD driver bug, though it would be nice to implement a workaround that doesn't use transfering
			if (!(surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)) {
				std::cerr << "swap chain image does not support VK_IMAGE_TRANSFER_DST usage" << std::endl;
				//exit(1);
			}

			// Determine transformation to use (preferring no transform)
			VkSurfaceTransformFlagBitsKHR surfaceTransform;
			if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
				surfaceTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else {
				surfaceTransform = surfaceCapabilities.currentTransform;
			}



			// Choose presentation mode (preferring MAILBOX ~= triple buffering)
			VkPresentModeKHR presentMode = choosePresentMode(presentModes);

			// Finally, create the swap chain
			VkSwapchainCreateInfoKHR createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = windowSurface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = swapChainExtent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
			createInfo.preTransform = surfaceTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;
			createInfo.oldSwapchain = VK_NULL_HANDLE;

			if (vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
				std::cerr << "failed to create swap chain" << std::endl;
				exit(1);
			}
			else {
				std::cout << "created swap chain" << std::endl;
			}

			// Store the images used by the swap chain
			// Note: these are the images that swap chain image indices refer to
			// Note: actual number of images may differ from requested number, since it's a lower bound
			uint32_t actualImageCount = 0;
			if (vkGetSwapchainImagesKHR(m_Device, swapChain, &actualImageCount, nullptr) != VK_SUCCESS || actualImageCount == 0) {
				std::cerr << "failed to acquire number of swap chain images" << std::endl;
				exit(1);
			}

			swapChainImages.resize(actualImageCount);
			swapChainFramebuffers.resize(actualImageCount);

			if (vkGetSwapchainImagesKHR(m_Device, swapChain, &actualImageCount, swapChainImages.data()) != VK_SUCCESS) {
				std::cerr << "failed to acquire swap chain images" << std::endl;
				exit(1);
			}

			swapChainImageViews.resize(swapChainImages.size());

			for (size_t i = 0; i < swapChainImages.size(); i++) {
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = swapChainImages[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = swapChainImageFormat;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;

				if (vkCreateImageView(m_Device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to create image views!");
				}
			}

			std::cout << "acquired swap chain images" << std::endl;
		}

		void VKGraphicsContext::createRenderPass() {
			VkAttachmentDescription colorAttachment{};
			colorAttachment.format = swapChainImageFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkSubpassDependency dependency{};
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;
			renderPassInfo.dependencyCount = 1;
			renderPassInfo.pDependencies = &dependency;

			if (vkCreateRenderPass(m_Device, &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
				throw std::runtime_error("failed to create render pass!");
			}

			swapChainFramebuffers.resize(swapChainImageViews.size());

			for (size_t i = 0; i < swapChainImageViews.size(); i++) {
				VkImageView attachments[] = {
					swapChainImageViews[i]
				};

				VkFramebufferCreateInfo framebufferInfo{};
				framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				framebufferInfo.renderPass = m_RenderPass;
				framebufferInfo.attachmentCount = 1;
				framebufferInfo.pAttachments = attachments;
				framebufferInfo.width = 1280;
				framebufferInfo.height = 720;
				framebufferInfo.layers = 1;

				if (vkCreateFramebuffer(m_Device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to create framebuffer!");
				}
			}
		}

		static std::vector<char> readFile(const std::string& filename) {
			std::ifstream file(filename, std::ios::ate | std::ios::binary);

			if (!file.is_open()) {
				throw std::runtime_error("failed to open file!");
			}

			size_t fileSize = (size_t)file.tellg();
			std::vector<char> buffer(fileSize);

			file.seekg(0);
			file.read(buffer.data(), fileSize);

			file.close();

			return buffer;
		}

		void VKGraphicsContext::createGraphicsPipeline() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(float) * 5;
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			VkVertexInputAttributeDescription attributeDescriptions[2] = {};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = 0;

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = sizeof(float) * 2;

			VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(2);
			vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
			vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions;

			auto vertShaderCode = readFile("./assets/shaders/vert.spv");
			auto fragShaderCode = readFile("./assets/shaders/frag.spv");

			VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
			VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

			VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";

			VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

			VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float) 1280;
			viewport.height = (float) 720;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{};
			scissor.offset = { 0, 0 };
			scissor.extent = { 1280, 720 };

			VkPipelineViewportStateCreateInfo viewportState{};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizer{};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling{};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending{};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pushConstantRangeCount = 0;

			if (vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
				throw std::runtime_error("failed to create pipeline layout!");
			}

			VkGraphicsPipelineCreateInfo pipelineInfo{};
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = m_PipelineLayout;
			pipelineInfo.renderPass = m_RenderPass;
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			if (vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS) {
				throw std::runtime_error("failed to create graphics pipeline!");
			}

			vkDestroyShaderModule(m_Device, fragShaderModule, nullptr);
			vkDestroyShaderModule(m_Device, vertShaderModule, nullptr);
		}

		VkShaderModule VKGraphicsContext::createShaderModule(const std::vector<char>& code) {
			VkShaderModuleCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size();
			createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

			VkShaderModule shaderModule;
			if (vkCreateShaderModule(m_Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
				throw std::runtime_error("failed to create shader module!");
			}

			return shaderModule;
		}

		void VKGraphicsContext::CreateCommandQueues()
		{
			// Setup fences
			FenceDescriptor fenceDesc = { };
			fenceDesc.Name = "acuireFence";
			
			acuireFence = Fence::Create(&fenceDesc);
			acuireFence->Reset();

			fenceDesc.Name = "submitFence";
			submitFence = Fence::Create(&fenceDesc);
			submitFence->Reset();

			// Setup the descriptor
			CommandPoolDescriptor commandPoolDesc = { };
			commandPoolDesc.Name = "MainCommandPool";
			commandPoolDesc.Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;

			// Create the commandAllocator
			commandPool = CommandPool::Create(&commandPoolDesc);

			// Setup the descriptor
			CommandBufferDescriptor commandBufferDesc = { };
			commandBufferDesc.Name = "";
			commandBufferDesc.Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
			commandBufferDesc.CommandPool = commandPool;

			// Setup the descriptor
			CommandQueueDescriptor commandQueueDesc = { };
			commandQueueDesc.Name = "mainCommandQueue";
			commandQueueDesc.QueueIndex = 0;
			commandQueueDesc.Type = CommandQueueType::NEON_COMMAND_QUEUE_TYPE_DIRECT;

			// Create the commandQueue
			commandQueue = CommandQueue::Create(&commandQueueDesc);


			// Create command buffer
			for (int i = 0; i < swapChainImages.size(); ++i)
				commandBuffers.push_back(CommandBuffer::Create(&commandBufferDesc));

			float vertices[] = {
				 0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
				 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			};

			MemoryPoolDescriptor memoryPoolDesc = {};
			memoryPoolDesc.Name = "VertexMemoryPool";
			memoryPoolDesc.Access = MemoryAccess::NEON_MEMORY_ACCESS_GPU_ONLY;
			memoryPoolDesc.Size = sizeof(vertices);

			memoryPool = MemoryPool::Create(&memoryPoolDesc);

			VertexBufferDescriptor vertexBufferDesc = {};
			vertexBufferDesc.Name = "VertexBuffer";
			vertexBufferDesc.Size = sizeof(vertices);
			vertexBufferDesc.Usage = BufferUsage::NEON_BUFFER_USAGE_STATIC;
			vertexBufferDesc.Vertices = vertices;
			vertexBufferDesc.VMemoryPool = memoryPool;
			vertexBufferDesc.VertexCount = 3;

			vertexBuffer = VertexBuffer::Create(commandBuffers[0], &vertexBufferDesc);

			// Note: contains value for each subresource range
			VkClearColorValue clearColor = {
				{ 0.4f, 0.6f, 0.9f, 1.0f } // R, G, B, A
			};

			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subResourceRange.baseMipLevel = 0;
			subResourceRange.levelCount = 1;
			subResourceRange.baseArrayLayer = 0;
			subResourceRange.layerCount = 1;

			// Record the command buffer for every swap chain image
			for (uint32_t i = 0; i < swapChainImages.size(); i++) {
				// Change layout of image to be optimal for clearing
				// Note: previous layout doesn't matter, which will likely cause contents to be discarded
				VkImageMemoryBarrier presentToClearBarrier = {};
				presentToClearBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				presentToClearBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				presentToClearBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				presentToClearBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				presentToClearBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				presentToClearBarrier.srcQueueFamilyIndex = presentQueueFamily;
				presentToClearBarrier.dstQueueFamilyIndex = presentQueueFamily;
				presentToClearBarrier.image = swapChainImages[i];
				presentToClearBarrier.subresourceRange = subResourceRange;

				// Change layout of image to be optimal for presenting
				VkImageMemoryBarrier clearToPresentBarrier = {};
				clearToPresentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				clearToPresentBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				clearToPresentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				clearToPresentBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				clearToPresentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				clearToPresentBarrier.srcQueueFamilyIndex = presentQueueFamily;
				clearToPresentBarrier.dstQueueFamilyIndex = presentQueueFamily;
				clearToPresentBarrier.image = swapChainImages[i];
				clearToPresentBarrier.subresourceRange = subResourceRange;

				VkRenderPassBeginInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = m_RenderPass;
				renderPassInfo.framebuffer = swapChainFramebuffers[i];
				renderPassInfo.renderArea.offset = { 0, 0 };
				renderPassInfo.renderArea.extent = { 1280, 720};

				VkClearValue clearColor1 = { 0.0f, 0.0f, 0.0f, 1.0f };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor1;

				// Record command buffer
				commandBuffers[i]->StartRecording();

			//	vkCmdClearColorImage(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, swapChainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subResourceRange);


				vkCmdBeginRenderPass(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

				vkCmdBindPipeline(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

				VkBuffer vertexBuffers[] = { NEON_CAST(VKVertexBuffer*, vertexBuffer)->m_VertexBufferObj };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, 0, 1, vertexBuffers, offsets);

			//	vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);


				vkCmdDraw(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, vertexBuffer->GetVertexCount(), 1, 0, 0);

				vkCmdEndRenderPass(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj);


				vkCmdPipelineBarrier(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &presentToClearBarrier);

				vkCmdPipelineBarrier(NEON_CAST(VKCommandBuffer*, commandBuffers[i])->m_CommandBufferObj, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &clearToPresentBarrier);

				commandBuffers[i]->EndRecording();
			}
		}

		bool VKGraphicsContext::CheckLayersSupport(const std::vector<const char*> _VKLayers)
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			VkLayerProperties *layersAvailable = new VkLayerProperties[layerCount];
			vkEnumerateInstanceLayerProperties(&layerCount, layersAvailable);

			int layersFound = 0;
			for (int i = 0; i < layerCount; ++i) {
				for (int j = 0; j < _VKLayers.size(); ++j)
				{
					if (strcmp(layersAvailable[i].layerName, _VKLayers[j]) == 0) {
						layersFound++;
					}
				}
			}

			if (layersFound < _VKLayers.size())
				return false;

			return true;
		}
		bool VKGraphicsContext::CheckExtensionsSupport(const std::vector<const char*> _VKExtensions)
		{
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
			VkExtensionProperties* extensionsAvailable = new VkExtensionProperties[extensionCount];
			vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionsAvailable);

			int extensionsFound = 0;
			for (int i = 0; i < extensionCount; ++i)
			{
				for (int j = 0; j < _VKExtensions.size(); ++j)
				{
					if (strcmp(extensionsAvailable[i].extensionName, _VKExtensions[j]) == 0) {
						extensionsFound++;
					}
				}
			}

			if (extensionsFound < _VKExtensions.size())
				return false;

			return true;
		}

		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
			printf("validation layer: %s\n", pCallbackData->pMessage);

			return VK_FALSE;
		}
	}
}