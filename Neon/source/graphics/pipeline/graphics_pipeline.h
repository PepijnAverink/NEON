#pragma once
#include "./graphics/pipeline/graphics_pipeline_descriptor.h"

#include "./graphics/objects/command_generic/viewport.h"
#include "./graphics/pipeline/inputLayout/input_layout.h"
#include "./graphics/pipeline/topology/topology.h"

namespace Neon
{
	namespace Graphics
	{
		class GraphicsPipeline
		{
		public:
			static GraphicsPipeline* Create(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor);
			virtual ~GraphicsPipeline() {}

		protected:
			GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor);

			InputLayout m_InputLayout;
			Topology    m_Topology;

			unsigned int m_ImageWidth;
			unsigned int m_ImageHeight;
			Viewport m_Viewport;
			Scissor  m_Scissor;

			RasterizerState m_RasterizerState;
		};
	}
}