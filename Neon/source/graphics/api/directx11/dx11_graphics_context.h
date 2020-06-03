#pragma once
#include "./graphics/graphics_context.h"

#include <D3D11.h>
#include <D3D11_4.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11GraphicsContext : public GraphicsContext
		{
		public:
			DX11GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor);
			virtual ~DX11GraphicsContext();

			virtual bool Initialize() override;
			virtual bool Terminate()  override;

			// Get's a Dx12 instance
			static DX11GraphicsContext* GetInstance() { return (DX11GraphicsContext*)GetGraphicsContext(); }


			inline ID3D11Device5*               GetGraphicsDevice() const { return m_Device; }
			inline ID3D11DeviceContext4* GetGraphicsDeviceContext() const { return m_DeviceContext; }

			inline IDXGIFactory*					   GetFactory() const { return m_Factory; }

		private:
			void GetAdapters();
			void CreateDevice();

			ID3D11Device5*			 m_Device;
			ID3D11DeviceContext4*	 m_DeviceContext;

			IDXGIFactory*			 m_Factory;
		};
	}
}