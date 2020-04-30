#pragma once
#include "./graphics/objects/command_generic/viewport.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12Viewport : public Viewport
		{
		public:
			DX12Viewport(const float _bottomX, const float _bottomY, const float _width, const float _height);
			virtual ~DX12Viewport();

			virtual void SetViewport(const float _bottomX, const float _bottomY, const float _width, const float _height) override;

		private:
			friend class DX12CommandBuffer;
			D3D12_VIEWPORT m_InternalViewport;
		};
	}
}