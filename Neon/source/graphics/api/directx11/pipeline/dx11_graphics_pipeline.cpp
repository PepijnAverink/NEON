#include "./graphics/api/directx11/pipeline/dx11_graphics_pipeline.h"

namespace Neon
{
	namespace Graphics
	{
		DX11GraphicsPipeline::DX11GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: GraphicsPipeline(_graphicsPipelineDescriptor)
		{ }

		DX11GraphicsPipeline::~DX11GraphicsPipeline()
		{

		}
	}
}