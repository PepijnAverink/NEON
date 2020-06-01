#pragma once
#include "./graphics/objects/swapchain/swapchain_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class Fence;
		class CommandQueue;
		class FramebufferAttachment;
		class Swapchain
		{
		public:
			static Swapchain* Create(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~Swapchain() {}

			virtual FramebufferAttachment* GetFramebufferAttachment(const int _i) const = 0;

			virtual void Resize(const int _width, const int _height) = 0;

			virtual unsigned int  AcquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence) = 0;
			virtual void Present(CommandQueue* _commandQueue, const bool _vsync) const = 0;

			// Getters
			inline int GetCurrentFrameIndex() const { return m_CurrentFrameIndex; }
			inline int GetBackBufferCount()   const { return m_BackBufferCount; }

		protected:
			Swapchain(const SwapchainDescriptor* _swapchainDescriptor);

			BackBufferFormat m_Format;

			unsigned int m_BackBufferCount;
			unsigned int m_CurrentFrameIndex;

			int m_Width;
			int m_Height;
		};
	}
}