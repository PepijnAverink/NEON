#pragma once
#include "./graphics/objects/swapchain/swapchain_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class Fence;
		class CommandQueue;
		class Swapchain
		{
		public:
			static Swapchain* Create(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~Swapchain() {}

			virtual void Resize(const int _width, const int _height) = 0;
			virtual int  AquireNewImage(Fence* _signalFence) = 0;

		protected:
			Swapchain(const SwapchainDescriptor* _swapchainDescriptor);

			BackBufferFormat m_Format;

			int m_BackBufferCount;
			int m_CurrentFrameIndex;

			int m_Width;
			int m_Height;
		};
	}
}