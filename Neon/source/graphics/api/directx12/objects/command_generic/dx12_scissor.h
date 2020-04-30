#pragma once
#include "./graphics/objects/command_generic/scissor.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12Scissor : public Scissor
		{
		public:
			DX12Scissor(const float _bottomX, const float _bottomY, const float _width, const float _height);
			virtual ~DX12Scissor();

			virtual void SetScissor(const float _bottomX, const float _bottomY, const float _width, const float _height) override;

		private:
			friend class DX12CommandBuffer;
			D3D12_RECT m_InternalScissor;
		};
	}
}