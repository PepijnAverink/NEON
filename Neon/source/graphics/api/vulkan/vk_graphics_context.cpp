#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./core/window/window.h"

#include "./graphics/objects/framebuffer/framebuffer_attachment_transition_state.h"

// Some defines
#include "./core/core_defines.h"
#include "./graphics/api/vulkan/vk_error.h"

#include <assert.h>
#include <vector>
#include <iostream>

#include <fstream>

#include "./graphics/api/vulkan/resources/shader/vk_shader.h"

#include "./graphics/objects/framebuffer/framebuffer_clear_flags.h"

namespace Neon
{
	namespace Graphics
	{
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

		HWND hwnd;
		VkDebugUtilsMessengerEXT debugMessenger;

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

				swapChainExtent.width  = min(max(640, surfaceCapabilities.minImageExtent.width), surfaceCapabilities.maxImageExtent.width);
				swapChainExtent.height = min(max(480, surfaceCapabilities.minImageExtent.height), surfaceCapabilities.maxImageExtent.height);

				return swapChainExtent;
			}
			else {
				return surfaceCapabilities.currentExtent;
			}
		}

		void VKGraphicsContext::findQueueFamilies(const GraphicsContextDescriptor* _graphicsContextDescriptor) {

			// Temporary surface
			GraphicsSurfaceDescriptor graphicsSurfaceDesc = {};
			graphicsSurfaceDesc.Window = _graphicsContextDescriptor->Window;
		
			VKGraphicsSurface* surface = NEON_CAST(VKGraphicsSurface*, GraphicsSurface::Create(&graphicsSurfaceDesc));

			// Get the family count
			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, nullptr);

			if (queueFamilyCount == 0) {
				printf("[ERROR] VKGraphicsContext::findQueueFamilies() - No queue families found on the current device.\n");
				return;
			}

			// Get queue properties
			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(m_PhysicalDevice, &queueFamilyCount, queueFamilies.data());

			// Setup queues
			for (uint32_t i = 0; i < queueFamilyCount; i++) {
				VkBool32 presentSupport = true;
				vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i,surface->m_WindowSurfaceObj, &presentSupport);

				if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					m_GraphicsFamily = i;
					if (presentSupport)
						m_PresentFamily = i;
				}
			}

			// release temp surface
			delete surface;
		}

		VKGraphicsContext::VKGraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor)
			: GraphicsContext(_graphicsContextDescriptor)
		{
			// Remove me
			hwnd = (HWND)_graphicsContextDescriptor->Window->GetNativeWindowHandle();

			CreateInstance();

			FindPhysicalDevice();
			CheckSwapchainSupport();

			SetDebugUtilsObjectName = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(m_Instance, "vkSetDebugUtilsObjectNameEXT");


			findQueueFamilies(_graphicsContextDescriptor);
			CreateLogicalDevice(_graphicsContextDescriptor);
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

#if defined(NEON_DEBUG)
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
			func(m_Instance, debugMessenger, nullptr);
#endif

			vkDestroyInstance(m_Instance, nullptr);
			return false;
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
			VK_ThrowIfFailed(vkCreateInstance(&createInfo, nullptr, &m_Instance));

#if defined(NEON_DEBUG)
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkCreateDebugUtilsMessengerEXT");
			func(m_Instance, &debugCreateInfo, nullptr, &debugMessenger);
#endif
		}

		// TODO:: find best device
		void VKGraphicsContext::FindPhysicalDevice()
		{
			// Try to find 1 Vulkan supported device
			uint32_t deviceCount = 1;
			VkResult res = vkEnumeratePhysicalDevices(m_Instance, &deviceCount, &m_PhysicalDevice);
			if (res != VK_SUCCESS && res != VK_INCOMPLETE) {
				std::cerr << "enumerating physical devices failed!" << std::endl;
				exit(1);
			}

			if (deviceCount == 0) {
				std::cerr << "no physical devices that support vulkan!" << std::endl;
				exit(1);
			}

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

		uint32_t VKGraphicsContext::QueueFamilyIDHelper(CommandQueueLayoutElement _element)
		{
			switch (_element.Type)
			{
				case NEON_COMMAND_QUEUE_TYPE_DIRECT: 
				{
					if (_element.PresentSupport)
						return m_PresentFamily;
					else
						return m_GraphicsFamily;
				}
			}

			return m_GraphicsFamily;
		}

		void VKGraphicsContext::CreateLogicalDevice(const GraphicsContextDescriptor* _graphicsContextDescriptor)
		{
			float queuePriority = 1.0f;

			std::vector<VkDeviceQueueCreateInfo> queueInfo;
			for (CommandQueueLayoutElement e : _graphicsContextDescriptor->QueueLayout.GetElements())
			{
				VkDeviceQueueCreateInfo info = {};
				info.sType				= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				info.queueFamilyIndex	= QueueFamilyIDHelper(e);
				info.queueCount			= e.Count;
				info.pQueuePriorities	= &queuePriority;

				queueInfo.push_back(info);
			}

			// Create logical device from physical device
			VkDeviceCreateInfo deviceCreateInfo		= {};
			deviceCreateInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceCreateInfo.pQueueCreateInfos		= queueInfo.data();
			deviceCreateInfo.queueCreateInfoCount   = queueInfo.size();

			// Add extension
			const char* deviceExtensions			 = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
			deviceCreateInfo.enabledExtensionCount	 = 1;
			deviceCreateInfo.ppEnabledExtensionNames = &deviceExtensions;

			VK_ThrowIfFailed(vkCreateDevice(m_PhysicalDevice, &deviceCreateInfo, nullptr, &m_Device));
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
					printf("[LAYER] -%s\n", layersAvailable[i].layerName);
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