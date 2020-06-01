#pragma once
#include "./graphics/objects/command_generic/viewport.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11Viewport : public Viewport
		{
		public:
			DX11Viewport(const float _bottomX, const float _bottomY, const float _width, const float _height);
			virtual ~DX11Viewport();

			virtual void SetViewport(const float _bottomX, const float _bottomY, const float _width, const float _height) override;

		private:
			friend class DX11CommandBuffer;
			D3D11_VIEWPORT m_InternalViewport;
		};
	}
}