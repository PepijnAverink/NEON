#pragma once
#include "./graphics/resources/texture/texture2D.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11Texture2D : public Texture2D
		{
		public:
			DX11Texture2D(CommandBuffer* _commandBuffer, const Texture2DDescriptor* _texture2DDectiptor);

		private:
			ID3D11Texture2D* m_texture;
			ID3D11ShaderResourceView* m_textureView;
		};
	}
}