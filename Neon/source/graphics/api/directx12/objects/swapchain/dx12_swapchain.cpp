#include "./graphics/api/directx12/objects/swapchain/dx12_swapchain.h"

namespace Neon
{
	namespace Graphics
	{
		DX12Swapchain::DX12Swapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor)
			: Swapchain(_swapchainDescriptor)
		{

		}

		DX12Swapchain::~DX12Swapchain()
		{

		}
		void DX12Swapchain::Resize(const int _width, const int _height)
		{

		}

		int DX12Swapchain::AquireNewImage(Fence * _signalFence)
		{
			return 0;
		}
	}
}