#pragma once
#include <string>

namespace Neon
{
	namespace Graphics
	{
		class ComputeShaderDescriptor
		{
		public:
			ComputeShaderDescriptor() = default;

			std::string ComputeShaderPath;
			std::string ComputeShaderFunctionName;

			uint32_t    ShaderFlags; // TODO:: implement
			bool		HotReload;
		};
	}
}