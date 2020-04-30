#include "./graphics/api/directx12/objects/command_generic/dx12_viewport.h"

namespace Neon
{
	namespace Graphics
	{
		DX12Viewport::DX12Viewport(const float _bottomX, const float _bottomY, const float _width, const float _height)
			: Viewport(_bottomX, _bottomY, _width, _height)
		{
			m_InternalViewport = { m_BottomX, m_BottomY, m_Width, m_Height, m_MinDepth, m_MaxDepth };
		}

		DX12Viewport::~DX12Viewport()
		{ }

		void DX12Viewport::SetViewport(const float _bottomX, const float _bottomY, const float _width, const float _height)
		{
			// Set members
			m_BottomX = _bottomX;
			m_BottomY = _bottomY;
			m_Width  = _width;
			m_Height = _height;

			m_InternalViewport = { m_BottomX, m_BottomY, m_Width, m_Height, m_MinDepth, m_MaxDepth};
		}
	}
}