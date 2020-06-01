#pragma once
#include "./graphics/pipeline/graphics_pipeline.h"
#include "./graphics/api/directx11/resources/shader/dx11_shader.h"

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

			ID3D11RasterizerState*   m_RasterState;
			ID3D11DepthStencilState* m_DepthStencilState;
			ID3D11InputLayout*		 m_InputLayout;

			DX11Shader*				 m_InternalShader;
		};
	}
}