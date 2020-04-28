#pragma once
#include "./graphics/objects/command_generic/viewport.h"
#include "./graphics/objects/command_generic/scissor.h"
#include "./graphics/pipeline/topology/topology.h"

#include "./graphics/pipeline/rasterizer/rasterizer_state.h"

namespace Neon
{
	namespace Graphics
	{
		class GraphicsPipelineDescriptor
		{
		public:
			GraphicsPipelineDescriptor() = default;

			int		 ImageWidth;
			int		 ImageHeight;
			Viewport ViewportRect;
			Scissor  ScissorRect;

			Topology DrawTopology;

			RasterizerState RasterizerStateDescriptor;
		};
	}
}