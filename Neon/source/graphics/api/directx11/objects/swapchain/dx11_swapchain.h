#pragma once
#include "./graphics/objects/swapchain/swapchain.h"

#include <dxgi1_4.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11Swapchain : public Swapchain
		{
		public:
			DX11Swapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~DX11Swapchain();

			virtual FramebufferAttachment* GetFramebufferAttachment(const int _i) const override;

			virtual void Resize(const int _width, const int _height) override;

			virtual int  AquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence) override;
			virtual void Present(CommandQueue* _commandQueue, const bool _vsync) const override;

		private:
			friend class DX11GraphicsContext; // Remove me
			IDXGISwapChain3* m_SwapchainObj;
		};
	}
}