#pragma once
#include "./graphics/objects/swapchain/swapchain_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class Swapchain
		{
		public:
			static Swapchain* Create(const SwapchainDescriptor* _swapchainDescriptor);
			virtual ~Swapchain() {}

			virtual void Resize(const int _width, const int _height) = 0;

		protected:

			int m_BackBufferCount;

			int m_Width;
			int m_Height;
		};
	}
}