#pragma once
#include "./graphics/pipeline/graphics_pipeline.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11GraphicsPipeline : public GraphicsPipeline
		{
		public:
			DX11GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor);
			virtual ~DX11GraphicsPipeline();

		private:
			friend class DX11CommandBuffer;
			friend class DX11GraphicsContext; // TODO:: Remove me

			ID3D11RasterizerState*   m_RasterState;
			ID3D11DepthStencilState* m_DepthStencilState;
		};
	}
}