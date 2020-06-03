#pragma once
#include "./graphics/graphics_context.h"
#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12GraphicsContext final : public GraphicsContext
		{
		public:
			DX12GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor);
			virtual ~DX12GraphicsContext();

			virtual bool Initialize() override;
			virtual bool Terminate()  override;

			// Get's a Dx12 instance
			static DX12GraphicsContext* GetInstance() { return (DX12GraphicsContext*)GetGraphicsContext(); }

			inline ID3D12Device* GetGraphicsDevice() const { return m_Device; }

		private:

			ID3D12Device* m_Device;
		};
	}
}