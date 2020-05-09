#pragma once
#include "./graphics/objects/swapchain/swapchain.h"

#include <dxgi1_4.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12Swapchain : public Swapchain
		{
		public:
			DX12Swapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~DX12Swapchain();

			virtual FramebufferAttachment* GetFramebufferAttachment(const int _i) const override;

			virtual void Resize(const int _width, const int _height) override;

			virtual int  AquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence) override;
			virtual void Present(CommandQueue* _commandQueue, const bool _vsync) const override;

		private:
			friend class DX12GraphicsContext; // Remove me
			IDXGISwapChain3* m_SwapChainObj;
		};
	}
}