#pragma once
#include "./graphics/resources/shader/shader_descriptor.h"
#include "./graphics/resources/shader/shader_reflection.h"
#include "./graphics/resources/shader/shader_flags.h"

namespace Neon
{
	namespace Graphics
	{
		class Shader
		{
		public:
			static Shader* Create(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor);
			virtual ~Shader() {}

		protected:
			Shader(const ShaderDescriptor* _shaderDescriptor);

			std::string m_VertexShaderPath;
			std::string m_VertexFunctionName;

			std::string m_FragmentShaderPath;
			std::string m_FragmentFunctionName;

			bool		m_HotReload;
		};
	}
}