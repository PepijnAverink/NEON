#pragma once
#include "./graphics/resources/shader/shader.h"

#include <d3d12.h>
#include <D3Dcompiler.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12Shader : public Shader
		{
		public:
			DX12Shader(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor);
			virtual ~DX12Shader();

		private:
			void ReflectInputLayout(InputLayout* _inputLayout, ID3D12ShaderReflection* _reflection, int a_inputParameterCount);

			friend class DX12GraphicsPipeline;
			friend class DX12GraphicsContext; // Remove me
			ID3DBlob* m_VertexShaderBytes;
			ID3DBlob* m_FragmentShaderBytes;
		};
	}
}