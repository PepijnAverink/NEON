#pragma once
#include "./graphics/objects/command_generic/viewport.h"
#include "./graphics/objects/command_generic/scissor.h"

#include "./graphics/pipeline/rasterizer/rasterizer_state.h"
#include "./graphics/pipeline/inputLayout/input_layout.h"

namespace Neon
{
	namespace Graphics
	{
		class Shader;
		class GraphicsPipelineDescriptor
		{
		public:
			GraphicsPipelineDescriptor() = default;

			std::string	Name;

			int			ImageWidth;
			int			ImageHeight;

			InputLayout InputLayout;

			Shader*		Shader;

			RasterizerState RasterizerStateDescriptor;
		};
	}
}