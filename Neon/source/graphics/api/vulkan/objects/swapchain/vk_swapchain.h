#pragma once
#include "./graphics/objects/swapchain/swapchain.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class VKSwapchain : public Swapchain
		{
		public:
			VKSwapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~VKSwapchain();

			virtual FramebufferAttachment* GetFramebufferAttachment(const int _i) const override;

			virtual void Resize(const int _width, const int _height) override;

			virtual int  AquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence) override;
			virtual void Present(CommandQueue* _commandQueue, const bool _vsync) const override;

		private:
			friend class VKGraphicsContext; // Remove me
			VkSwapchainKHR		 m_SwapchainObj;

			std::vector<VkImage> m_SwapchainImages;
		};
	}
}