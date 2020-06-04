#pragma once
#include <string>

namespace Neon
{
	namespace Graphics
	{
		class ComputeShader;
		class ComputeGraphicsPipelineDescriptor
		{
		public:
			ComputeGraphicsPipelineDescriptor() = default;

			ComputeShader* Shader;

		};
	}
}