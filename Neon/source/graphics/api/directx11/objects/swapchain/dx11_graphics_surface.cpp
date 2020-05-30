#include "./graphics/api/directx11/objects/swapchain/dx11_graphics_surface.h"
#include "./core/window/window.h"

namespace Neon
{
	namespace Graphics
	{
		DX11GraphicsSurface::DX11GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor)
			: GraphicsSurface(_graphicsSurfaceDescriptor)
		{
			// Store windowHandle
			m_WindowHandle = _graphicsSurfaceDescriptor->Window->GetNativeWindowHandle();
		}
	}
}
