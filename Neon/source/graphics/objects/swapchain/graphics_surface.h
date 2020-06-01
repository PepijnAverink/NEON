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
			virtual ~GraphicsSurface() {}

		protected:
			GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor);
		};
	}
}