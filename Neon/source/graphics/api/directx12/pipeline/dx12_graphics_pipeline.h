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
			ID3D12PipelineState* m_PipelineStateObject;
		};
	}
}