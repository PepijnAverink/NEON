#pragma once
#include "./graphics/graphics_context.h"

#include <vector>
#include <string>

// Vulkan
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

// Graphics abstraction
#include "./utilities/casting/casting_helper.h"
#include "./graphics/api/vulkan/objects/sync/vk_fence.h"
#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
#include "./graphics/api/vulkan/objects/command/vk_command_queue.h"

#include "./graphics/resources/memory/memory_pool.h"
#include "./graphics/api/vulkan/resources/buffer/vk_vertex_buffer.h"
#include "./graphics/api/vulkan/resources/buffer/vk_index_buffer.h"

#include "./graphics/api/vulkan/pipeline/vk_graphics_pipeline.h"

#include "./graphics/api/vulkan/objects/swapchain/vk_swapchain.h"
#include "./graphics/api/vulkan/objects/swapchain/vk_graphics_surface.h"

#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer.h"
#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		class VKGraphicsContext final : public GraphicsContext
		{
		public:
			VKGraphicsContext(Core::Window* _window);
			virtual ~VKGraphicsContext();

			virtual bool Initialize() override;
			virtual bool Terminate()  override;

			virtual void Present() override;

			// Get's a VK instance
			static VKGraphicsContext* GetInstance() { return (VKGraphicsContext*)GetGraphicsContext(); }

			inline VkDevice			GetGraphicsDevice() const { return m_Device; }
			inline VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }

			inline VkInstance     GetGraphicsInstance() const { return m_Instance; }

			inline PFN_vkSetDebugUtilsObjectNameEXT GetDebugNameUtils() const { return SetDebugUtilsObjectName; }

			inline uint32_t GetQueueFamilyIDGraphics() const { return m_GraphicsQueueFamilyID; }

		private:
			void CreateInstance();
			void CreateSurface();
			void FindPhysicalDevice();
			bool CheckSwapchainSupport();
			void CreateLogicalDevice();
			void CreateSwapchain();
			void CreateCommandQueues();
			void createGraphicsPipeline();
			void findQueueFamilies();

			VkShaderModule createShaderModule(const std::vector<char>& code);

			bool CheckLayersSupport(const std::vector<const char*> _VKLayers);
			bool CheckExtensionsSupport(const std::vector<const char*> _VKExtensions);

			VkInstance m_Instance;

			VkDevice		 m_Device;
			VkPhysicalDevice m_PhysicalDevice;

			uint32_t m_GraphicsQueueFamilyID;

			// Abstraction
			CommandPool* commandPool;
			CommandQueue* commandQueue;
			std::vector<CommandBuffer*> commandBuffers;

			Fence* submitFence;
			Fence* acuireFence;

			PFN_vkSetDebugUtilsObjectNameEXT SetDebugUtilsObjectName;

			VertexBuffer* vertexBuffer;
			MemoryPool*   memoryPool;

			IndexBuffer* indexBuffer;
			MemoryPool* imemoryPool;

			GraphicsPipeline* m_Pipeline;

			Swapchain*		 m_Swapchain;
			GraphicsSurface* m_Surface;

			Framebuffer*	  m_Framebuffer[frameBufferCount];
		//	FramebufferAttachment* m_FramebufferAttachments[frameBufferCount];
		};
	}
}