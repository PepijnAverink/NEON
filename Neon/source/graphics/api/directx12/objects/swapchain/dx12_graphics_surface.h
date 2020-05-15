#pragma once
#include "./graphics/objects/swapchain/graphics_surface.h"

namespace Neon
{
	namespace Graphics
	{
		class DX12GraphicsSurface : public GraphicsSurface
		{
		public:
		public:
			DX12GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor);

		private:
			friend class DX12Swapchain;
			void* m_WindowHandle = nullptr;
		};
	}
}