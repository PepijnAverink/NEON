#pragma once
#include "./graphics/objects/swapchain/graphics_surface_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class GraphicsSurface
		{
		public:
			static GraphicsSurface* Create(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor);

		protected:
			GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor);
		};
	}
}