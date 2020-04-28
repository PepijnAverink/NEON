#pragma once
#include "./graphics/resources/shader/shader_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class Shader
		{
		public:
			Shader* Create(const ShaderDescriptor* _shaderDescriptor);

		private:
			Shader(const ShaderDescriptor* _shaderDescriptor);

			std::string m_VertexShaderPath;
			std::string m_VertexFunctionName;

			std::string m_FragmentShaderPath;
			std::string m_FragmentFunctionName;
		};
	}
}