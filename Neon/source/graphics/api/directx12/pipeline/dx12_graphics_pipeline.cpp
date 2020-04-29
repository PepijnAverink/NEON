#include "./graphics/api/directx12/pipeline/dx12_graphics_pipeline.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/pipeline/inputLayout/dx12_input_layout.h"
#include "./graphics/api/directx12/dx12_error.h"

#include <DX12/d3dx12.h>

#include "./graphics/api/directx12/resources/shader/dx12_shader.h"

namespace Neon
{
	namespace Graphics
	{
		DX12GraphicsPipeline::DX12GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: GraphicsPipeline(_graphicsPipelineDescriptor)
		{
			// create root signature
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ID3DBlob* signature;
			DX12_ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));
			DX12_ThrowIfFailed(DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)));

			std::vector<D3D12_INPUT_ELEMENT_DESC> elements;
			GetDX12InputLayout(elements, &_graphicsPipelineDescriptor->InputLayout);

			// fill out an input layout description structure
			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
			inputLayoutDesc.NumElements = _graphicsPipelineDescriptor->InputLayout.GetElementCount();
			inputLayoutDesc.pInputElementDescs = elements.data();


			// fill out a shader bytecode structure
			D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
			vertexShaderBytecode.BytecodeLength  = NEON_CAST(DX12Shader*, _graphicsPipelineDescriptor->Shader)->m_VertexShaderBytes->GetBufferSize();
			vertexShaderBytecode.pShaderBytecode = NEON_CAST(DX12Shader*, _graphicsPipelineDescriptor->Shader)->m_VertexShaderBytes->GetBufferPointer();

			// fill out shader bytecode structure for pixel shader
			D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
			pixelShaderBytecode.BytecodeLength  = NEON_CAST(DX12Shader*, _graphicsPipelineDescriptor->Shader)->m_FragmentShaderBytes->GetBufferSize();
			pixelShaderBytecode.pShaderBytecode = NEON_CAST(DX12Shader*, _graphicsPipelineDescriptor->Shader)->m_FragmentShaderBytes->GetBufferPointer();

			DXGI_SAMPLE_DESC sampleDesc = {};
			sampleDesc.Count = 1;

			// create a pipeline state object (PSO)
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = inputLayoutDesc;
			psoDesc.pRootSignature = m_RootSignature;
			psoDesc.VS = vertexShaderBytecode;
			psoDesc.PS = pixelShaderBytecode;
			psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			psoDesc.SampleDesc = sampleDesc;
			psoDesc.SampleMask = 0xffffffff;
			psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.NumRenderTargets = 1;
			psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

			// create the pso
			DX12_ThrowIfFailed(DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_PipelineStateObject)));

			// Fill out the Viewport
			m_Viewport.TopLeftX = 0;
			m_Viewport.TopLeftY = 0;
			m_Viewport.Width    = _graphicsPipelineDescriptor->ImageWidth;
			m_Viewport.Height   = _graphicsPipelineDescriptor->ImageHeight;
			m_Viewport.MinDepth = 0.0f;
			m_Viewport.MaxDepth = 1.0f;

			// Fill out a scissor rect
			m_ScissorRect.left   = 0;
			m_ScissorRect.top    = 0;
			m_ScissorRect.right  = _graphicsPipelineDescriptor->ImageWidth;
			m_ScissorRect.bottom = _graphicsPipelineDescriptor->ImageHeight;
		}

		DX12GraphicsPipeline::~DX12GraphicsPipeline()
		{

		}
	}
}