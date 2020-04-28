#include "./graphics/resources/shader/shader.h"

namespace Neon
{
	namespace Graphics
	{
		Shader::Shader(const ShaderDescriptor* _shaderDescriptor)
			: m_VertexShaderPath(_shaderDescriptor->VertexShaderPath)
			, m_VertexFunctionName(_shaderDescriptor->VertexShaderFunctionName)
			, m_FragmentShaderPath(_shaderDescriptor->FragmentShaderPath)
			, m_FragmentFunctionName(_shaderDescriptor->FragmentShaderFunctionName)
		{ }
	}
}