#include "./graphics/api/directx11/pipeline/dx11_graphics_pipeline.h"
#include "./graphics/api/directx11/pipeline/inputLayout/dx11_input_layout.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX11GraphicsPipeline::DX11GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: GraphicsPipeline(_graphicsPipelineDescriptor)
		{ 
			D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
			depthStencilDesc.DepthEnable					= true;
			depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
			depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
			depthStencilDesc.StencilEnable					= true;
			depthStencilDesc.StencilReadMask				= 0xFF;
			depthStencilDesc.StencilWriteMask				= 0xFF;
			depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
			depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
			depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
			depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
			depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

			// Create the depth stencil state.
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencilState));


			D3D11_RASTERIZER_DESC rasterDesc = {};
			rasterDesc.AntialiasedLineEnable	= false;
			rasterDesc.CullMode					= D3D11_CULL_NONE;
			rasterDesc.DepthBias				= 0;
			rasterDesc.DepthBiasClamp			= 0.0f;
			rasterDesc.DepthClipEnable			= true;
			rasterDesc.FillMode					= D3D11_FILL_SOLID;
			rasterDesc.FrontCounterClockwise	= false;
			rasterDesc.MultisampleEnable		= false;
			rasterDesc.ScissorEnable			= false;
			rasterDesc.SlopeScaledDepthBias		= 0.0f;

			// Create the rasterizer state from the description we just filled out.
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateRasterizerState(&rasterDesc, &m_RasterState));

			std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
			GetDX11InputLayout(elements, &_graphicsPipelineDescriptor->InputLayout);

			DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateInputLayout(elements.data(), elements.size(), NEON_CAST(DX11Shader*, _graphicsPipelineDescriptor->Shader)->m_VertexShaderBuffer->GetBufferPointer(),
				NEON_CAST(DX11Shader*, _graphicsPipelineDescriptor->Shader)->m_VertexShaderBuffer->GetBufferSize(), &m_InputLayout);

			m_InternalShader = NEON_CAST(DX11Shader*, _graphicsPipelineDescriptor->Shader);
		}

		DX11GraphicsPipeline::~DX11GraphicsPipeline()
		{

		}
	}
}