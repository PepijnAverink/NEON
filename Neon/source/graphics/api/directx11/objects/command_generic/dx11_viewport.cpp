#include "./graphics/api/directx11/objects/command_generic/dx11_viewport.h"

namespace Neon
{
	namespace Graphics
	{
		DX11Viewport::DX11Viewport(const float _bottomX, const float _bottomY, const float _width, const float _height)
			: Viewport(_bottomX, _bottomY, _width, _height)
		{
			m_InternalViewport = { m_BottomX, m_BottomY, m_Width, m_Height, m_MinDepth, m_MaxDepth };
		}

		DX11Viewport::~DX11Viewport()
		{ }

		void DX11Viewport::SetViewport(const float _bottomX, const float _bottomY, const float _width, const float _height)
		{
			// Set members
			m_BottomX = _bottomX;
			m_BottomY = _bottomY;
			m_Width = _width;
			m_Height = _height;

			m_InternalViewport = { m_BottomX, m_BottomY, m_Width, m_Height, m_MinDepth, m_MaxDepth };
		}
	}
}