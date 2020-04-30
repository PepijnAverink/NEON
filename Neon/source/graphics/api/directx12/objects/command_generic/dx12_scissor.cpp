#include "./graphics/api/directx12/objects/command_generic/dx12_scissor.h"

namespace Neon
{
	namespace Graphics
	{
		DX12Scissor::DX12Scissor(const float _bottomX, const float _bottomY, const float _width, const float _height)
			: Scissor(_bottomX, _bottomY, _width, _height)
		{
			m_InternalScissor = { static_cast<LONG>(m_BottomX), static_cast<LONG>(m_BottomY), static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };
		}

		DX12Scissor::~DX12Scissor()
		{ }

		void DX12Scissor::SetScissor(const float _bottomX, const float _bottomY, const float _width, const float _height)
		{
			// Set members
			m_BottomX = _bottomX;
			m_BottomY = _bottomY;
			m_Width  = _width;
			m_Height = _height;

			m_InternalScissor = { static_cast<LONG>(m_BottomX), static_cast<LONG>(m_BottomY), static_cast<LONG>(m_Width), static_cast<LONG>(m_Height)};
		}
	}
}