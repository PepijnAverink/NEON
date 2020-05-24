#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./core/window/window.h"

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

		VkQueue graphicsQueue;

		uint32_t graphicsQueueFamily;
		uint32_t presentQueueFamily;

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
				vkGetPhysicalDeviceSurfaceSupportKHR(m_PhysicalDevice, i, NEON_CAST(VKGraphicsSurface*, m_Surface)->m_WindowSurfaceObj, &presentSupport);

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

			FindPhysicalDevice();
			CheckSwapchainSupport();

			SetDebugUtilsObjectName = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(m_Instance, "vkSetDebugUtilsObjectNameEXT");

			GraphicsSurfaceDescriptor graphicsSurfaceDesc = {};
			graphicsSurfaceDesc.Name   = "Main-GraphicsSurface";
			graphicsSurfaceDesc.Window = _window;

			m_Surface = GraphicsSurface::Create(&graphicsSurfaceDesc);


			findQueueFamilies();
			m_GraphicsQueueFamilyID = graphicsQueueFamily;

			CreateLogicalDevice();

			SwapchainDescriptor swapchainDesc = {};
			swapchainDesc.Name				= "Main-Swapchain";
			swapchainDesc.Width				= 1280;
			swapchainDesc.Height			= 720;
			swapchainDesc.BackBufferCount	= 3;
			swapchainDesc.Surface			= m_Surface;

			m_Swapchain = Swapchain::Create(commandQueue, &swapchainDesc);

			createGraphicsPipeline();

			CreateSwapchain();

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
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugUtilsMessengerEXT");
			func(m_Instance, debugMessenger, nullptr);
#endif

			vkDestroyInstance(m_Instance, nullptr);
			return false;
		}

		void VKGraphicsContext::Present()
		{
			// Acquire image
			uint32_t imageIndex;
			VkResult res = vkAcquireNextImageKHR(m_Device, NEON_CAST(VKSwapchain*, m_Swapchain)->m_SwapchainObj, UINT64_MAX,VK_NULL_HANDLE, NEON_CAST(VKFence*, acuireFence)->m_FenceObj, &imageIndex);
		//	VkResult res = vkAcquireNextImageKHR(m_Device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

			acuireFence->WaitForFence();
			acuireFence->Reset();

			imageIndex = 0;

			if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
				std::cerr << "failed to acquire image" << std::endl;
			//	exit(1);
			}

			commandQueue->ExecuteCommandBuffer(commandBuffers[imageIndex], submitFence);

			submitFence->WaitForFence();
			submitFence->Reset();

	
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		//	presentInfo.waitSemaphoreCount = 1;
		//	presentInfo.pWaitSemaphores = &renderingFinishedSemaphore;
			presentInfo.waitSemaphoreCount = 0;
			presentInfo.pWaitSemaphores = VK_NULL_HANDLE;
			

			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &NEON_CAST(VKSwapchain*, m_Swapchain)->m_SwapchainObj;
			presentInfo.pImageIndices = &imageIndex;

			res = vkQueuePresentKHR(NEON_CAST(VKCommandQueue*, commandQueue)->m_CommandQueueObj, &presentInfo);

			if (res != VK_SUCCESS) {
				std::cerr << "failed to submit present command buffer" << std::endl;
			}
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
			layers.push_back("VK_LAYER_LUNARG_standard_validation");
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

		void VKGraphicsContext::CreateSurface()
		{
		//	// Setup the surface
		//	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
		//	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		//	surfaceCreateInfo.hinstance = GetModuleHandle(NULL);
		//	surfaceCreateInfo.hwnd = (HWND)hwnd;
		//
		//	VK_ThrowIfFailed(vkCreateWin32SurfaceKHR(m_Instance, &surfaceCreateInfo, NULL, &windowSurface));
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

		void VKGraphicsContext::CreateSwapchain()
		{

			// Framebuffer
			FramebufferLayout framebufferLayout = { FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT, };
			
			// Setup FramebufferDescriptor
			FramebufferDescriptor framebufferDesc = {};
			framebufferDesc.Name = "Main-Framebuffer";
			framebufferDesc.Width = 1280;
			framebufferDesc.Height = 720;
			framebufferDesc.AttachmentCount = 1;
			framebufferDesc.DepthAttachment = false;
			framebufferDesc.Layout = framebufferLayout;
			


			for (int i = 0; i < frameBufferCount; ++i)
			{		
				// Create Framebuffer
				FramebufferAttachment** att = new FramebufferAttachment*[1];
				att[0] = m_Swapchain->GetFramebufferAttachment(i);
				framebufferDesc.Attachments = att;
		
				m_Framebuffer[i] = Framebuffer::Create(&framebufferDesc, m_Pipeline);
			}

			std::cout << "acquired swap chain images" << std::endl;
		}

		void VKGraphicsContext::createGraphicsPipeline()
		{

			// Setup ShaderDescriptor
			ShaderDescriptor shaderDesc				= {};
			shaderDesc.VertexShaderPath				= "./assets/shaders/vert.spv";
			shaderDesc.VertexShaderFunctionName		= "main";
			shaderDesc.FragmentShaderPath			= "./assets/shaders/frag.spv";
			shaderDesc.FragmentShaderFunctionName	= "main";
			shaderDesc.HotReload					= false;

			// Create shader object
			ShaderReflection reflection;
			Shader* shader = Shader::Create(reflection, &shaderDesc);

			RasterizerStateDescriptor rasterizerStateDesc = {};

			// Graphics Pipeline
			GraphicsPipelineDescriptor pipelineDesc = {};
			pipelineDesc.Name        = "Main-GraphicsPipeline";
			pipelineDesc.ImageWidth  = 1280;
			pipelineDesc.ImageHeight = 720;
			pipelineDesc.Shader		 = shader;
			pipelineDesc.RasterizerStateDescriptor = &rasterizerStateDesc;

			m_Pipeline = GraphicsPipeline::Create(&pipelineDesc);

			// Setup shader modules
			VkPipelineShaderStageCreateInfo shaderStages[] = { NEON_CAST(VKShader*, shader)->m_VertexStageInfo, NEON_CAST(VKShader*, shader)->m_FragmentStageInfo };
		}

		VkShaderModule VKGraphicsContext::createShaderModule(const std::vector<char>& code)
		{
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
			for (int i = 0; i < frameBufferCount; ++i)
				commandBuffers.push_back(CommandBuffer::Create(&commandBufferDesc));

			float vertices[] = {
				-0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				-0.5f, 0.5f, 1.0f, 1.0f, 1.0f
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
			

			uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };

			MemoryPoolDescriptor imemoryPoolDesc = {};
			imemoryPoolDesc.Name = "VertexMemoryPool";
			imemoryPoolDesc.Access = MemoryAccess::NEON_MEMORY_ACCESS_GPU_ONLY;
			imemoryPoolDesc.Size = sizeof(indices);

			imemoryPool = MemoryPool::Create(&imemoryPoolDesc);

			IndexBufferDescriptor indexBufferDesc = {};
			indexBufferDesc.Name = "IndexBuffer";
			indexBufferDesc.Size = sizeof(indices);
			indexBufferDesc.Usage = BufferUsage::NEON_BUFFER_USAGE_STATIC;
			indexBufferDesc.Indices = indices;
			indexBufferDesc.IMemoryPool = imemoryPool;
			indexBufferDesc.IndexCount = 6;

			indexBuffer = IndexBuffer::Create(commandBuffers[0], &indexBufferDesc);

			// Note: contains value for each subresource range
			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subResourceRange.baseMipLevel = 0;
			subResourceRange.levelCount = 1;
			subResourceRange.baseArrayLayer = 0;
			subResourceRange.layerCount = 1;

			// Record the command buffer for every swap chain image
			for (uint32_t i = 0; i < frameBufferCount; i++) {
				// Change layout of image to be optimal for clearing
				VkImageMemoryBarrier presentToClearBarrier = {};
				presentToClearBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				presentToClearBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				presentToClearBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				presentToClearBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				presentToClearBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				presentToClearBarrier.srcQueueFamilyIndex = presentQueueFamily;
				presentToClearBarrier.dstQueueFamilyIndex = presentQueueFamily;
				presentToClearBarrier.image = NEON_CAST(VKFramebufferAttachment*, m_Framebuffer[i]->GetAttachment(0))->m_Image;
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
				clearToPresentBarrier.image = NEON_CAST(VKFramebufferAttachment*, m_Framebuffer[i]->GetAttachment(0))->m_Image;
				clearToPresentBarrier.subresourceRange = subResourceRange;

				// Record command buffer
				commandBuffers[i]->StartRecording();

				
				commandBuffers[i]->SetGraphicsPipeline(m_Pipeline);

				const float clearColor[] = { 0.8f, 0.2f, 0.4f, 1.0f };
				commandBuffers[i]->ClearFrameBuffer(m_Framebuffer[i], clearColor, 0, 0, NEON_CLEAR_COLOR_BIT | NEON_CLEAR_DEPTH_STENCIL_BIT);


				commandBuffers[i]->BeginRenderpass(m_Framebuffer[i]);

				commandBuffers[i]->SetVertexBuffer(vertexBuffer);
				commandBuffers[i]->SetIndexBuffer(indexBuffer);

				commandBuffers[i]->DrawIndexed(6, 0, 0);

				commandBuffers[i]->EndRenderpass();


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