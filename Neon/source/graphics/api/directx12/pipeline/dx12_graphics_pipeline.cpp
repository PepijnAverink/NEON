#include "./graphics/api/directx12/pipeline/dx12_graphics_pipeline.h"

namespace Neon
{
	namespace Graphics
	{
		DX12GraphicsPipeline::DX12GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: GraphicsPipeline(_graphicsPipelineDescriptor)
		{

		}
		DX12GraphicsPipeline::~DX12GraphicsPipeline()
		{
		}
	}
}