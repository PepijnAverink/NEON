#pragma once
#include "./graphics/resources/shader/shader.h"

#include <D3D11.h>
#include <D3DCompiler.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11Shader : public Shader
		{
		public:
			DX11Shader(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor);
			virtual ~DX11Shader();

		private:
			void ReflectInputLayout(InputLayout* _inputLayout, ID3D11ShaderReflection* _reflection, int a_inputParameterCount);

			friend class DX11GraphicsPipeline;
			friend class DX11CommandBuffer;

			ID3D11VertexShader* m_VertexShader;
			ID3D10Blob*			m_VertexShaderBuffer = 0;

			ID3D11PixelShader*  m_FragmentShader;
			ID3D10Blob*			m_FragmentShaderBuffer = 0;
		};
	}
}