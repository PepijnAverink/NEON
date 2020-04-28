#pragma once

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class ShaderDescriptor
		{
		public:
			ShaderDescriptor() = default;

			std::string VertexShaderPath;
			std::string VertexShaderFunctionName;

			std::string FragmentShaderPath;
			std::string FragmentShaderFunctionName;
		};
	}
}