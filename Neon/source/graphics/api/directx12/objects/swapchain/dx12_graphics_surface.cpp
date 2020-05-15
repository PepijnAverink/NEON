#include "dx12_graphics_surface.h"

#include "./core/window/window.h"

namespace Neon
{
	namespace Graphics
	{
		DX12GraphicsSurface::DX12GraphicsSurface(const GraphicsSurfaceDescriptor* _graphicsSurfaceDescriptor)
			: GraphicsSurface(_graphicsSurfaceDescriptor)
		{
			// Store windowHandle
			m_WindowHandle = _graphicsSurfaceDescriptor->Window->GetNativeWindowHandle();
		}
	}
}
