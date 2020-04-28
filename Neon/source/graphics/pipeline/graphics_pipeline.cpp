#include "./graphics/pipeline/graphics_pipeline.h"

namespace Neon
{
	namespace Graphics
	{
		GraphicsPipeline* GraphicsPipeline::Create(const GraphicsPipelineDescriptor * _graphicsPipelineDescriptor)
		{
			return nullptr;
		}

		GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: m_ImageWidth(_graphicsPipelineDescriptor->ImageWidth)
			, m_ImageHeight(_graphicsPipelineDescriptor->ImageHeight)
			, m_Viewport(_graphicsPipelineDescriptor->ViewportRect)
			, m_Scissor(_graphicsPipelineDescriptor->ScissorRect)
			, m_Topology(_graphicsPipelineDescriptor->DrawTopology)
			, m_RasterizerState(_graphicsPipelineDescriptor->RasterizerStateDescriptor)
		{ }
	}
}