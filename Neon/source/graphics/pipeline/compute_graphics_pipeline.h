#pragma once
#include "./graphics/pipeline/compute_graphics_pipeline_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class ComputeGraphicsPipeline
		{
		public:
			static ComputeGraphicsPipeline* Create(const ComputeGraphicsPipelineDescriptor* _computeGraphicsPipeline);
			virtual ~ComputeGraphicsPipeline(){}

		private:
			ComputeGraphicsPipeline(const ComputeGraphicsPipelineDescriptor* _computeGraphicsPipeline);
		};
	}
}