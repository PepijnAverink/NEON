#pragma once
#include "./graphics/graphics_context.h"

#include <D3D11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11GraphicsContext : public GraphicsContext
		{
		public:
			DX11GraphicsContext(Core::Window* _window);
			virtual ~DX11GraphicsContext();

			virtual bool Initialize() override;
			virtual bool Terminate()  override;

			virtual void Present() override;

		private:
			void GetAdapters();
			void CreateDevice();

			ID3D11Device*			 m_Device;
			ID3D11DeviceContext*	 m_DeviceContext;
		};
	}
}