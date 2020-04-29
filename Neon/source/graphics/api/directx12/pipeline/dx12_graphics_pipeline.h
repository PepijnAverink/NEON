#pragma once
#include "./graphics/pipeline/graphics_pipeline.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12GraphicsPipeline : public GraphicsPipeline
		{
		public:
			DX12GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor);
			virtual ~DX12GraphicsPipeline();

		private:
			D3D12_VIEWPORT m_Viewport;
			D3D12_RECT     m_ScissorRect;

			friend class DX12CommandBuffer;
			friend class DX12GraphicsContext; // TODO:: Remove me
			ID3D12PipelineState* m_PipelineStateObject;
			ID3D12RootSignature* m_RootSignature;
		};
	}
}