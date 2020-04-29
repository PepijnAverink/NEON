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

			virtual void Resize(const int _width, const int _height) override;
			virtual int  AquireNewImage(Fence* _signalFence) override;

		private:
			IDXGISwapChain3* m_SwapChainObj;
		};
	}
}