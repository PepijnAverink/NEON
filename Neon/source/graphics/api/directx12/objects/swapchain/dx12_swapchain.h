#pragma once
#include "./graphics/objects/swapchain/swapchain.h"

namespace Neon
{
	namespace Graphics
	{
		class DX12Swapchain : public Swapchain
		{
		public:
			DX12Swapchain(const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~DX12Swapchain();


		};
	}
}