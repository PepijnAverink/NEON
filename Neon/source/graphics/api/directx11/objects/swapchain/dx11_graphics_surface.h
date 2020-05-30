#include "./graphics/objects/swapchain/graphics_surface.h"

namespace Neon
{
	namespace Graphics
	{
		class DX11GraphicsSurface : public GraphicsSurface
		{
		public:
		public:
			DX11GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor);

		private:
			friend class DX11Swapchain;
			void* m_WindowHandle = nullptr;
		};
	}
}