#pragma once
#include "./graphics/pipeline/graphics_pipeline.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKGraphicsPipeline : public GraphicsPipeline
		{
		public:
			VKGraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor);

		private:
			VkRenderPass	 m_RenderPass;
			VkPipelineLayout m_PipelineLayout;
			VkPipeline		 m_GraphicsPipeline;
		};
	}
}