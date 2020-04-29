#include "./graphics/api/directx12/resources/shader/dx12_shader.h"
#include "./graphics/api/directx12/pipeline/inputLayout/dx12_input_layout_element_format.h"

#include "./graphics/api/directx12/dx12_error.h"
#include "./utilities/string_utils.h"

#include <assert.h>

namespace Neon
{
	namespace Graphics
	{
		DX12Shader::DX12Shader(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor)
			: Shader(_shaderDescriptor)
		{
			HRESULT hr;
			ID3DBlob* errorBuff;

			// Compile VerteShader
			hr = D3DCompileFromFile(StringToWString(m_VertexShaderPath).c_str(), nullptr, nullptr, _shaderDescriptor->VertexShaderFunctionName.c_str(), "vs_5_0",
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_VertexShaderBytes, &errorBuff);
			if (FAILED(hr))
			{
				printf("[ERROR] DX12Shader - An error occured while compiling the VertexShader\n%s", errorBuff->GetBufferPointer());
				assert(false);
			//	OutputDebugStringA((char*)errorBuff->GetBufferPointer());
			}

			// compile FragmentShader
			hr = D3DCompileFromFile(StringToWString(_shaderDescriptor->FragmentShaderPath).c_str(), nullptr, nullptr, _shaderDescriptor->FragmentShaderFunctionName.c_str(), "ps_5_0", 
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &m_FragmentShaderBytes, &errorBuff);
			if (FAILED(hr))
			{
				printf("[ERROR] DX12Shader - An error occured while compiling the FragmentShader\n%s", errorBuff->GetBufferPointer());
				assert(false);
			//	OutputDebugStringA((char*)errorBuff->GetBufferPointer());
			}

			// Vertex shader reflection
			ID3D12ShaderReflection* vertexShaderReflection;
			DX12_ThrowIfFailed(D3DReflect(m_VertexShaderBytes->GetBufferPointer(), m_VertexShaderBytes->GetBufferSize(), IID_PPV_ARGS(&vertexShaderReflection)));

			D3D12_SHADER_DESC vertexShaderDesc;
			DX12_ThrowIfFailed(vertexShaderReflection->GetDesc(&vertexShaderDesc));

			// Reflect on the InputLayout
			ReflectInputLayout(&_shaderReflection.Layout, vertexShaderReflection, vertexShaderDesc.InputParameters);
		}

		DX12Shader::~DX12Shader()
		{
			m_VertexShaderBytes->Release();
			m_FragmentShaderBytes->Release();
		}

		void DX12Shader::ReflectInputLayout(InputLayout* _inputLayout, ID3D12ShaderReflection* _reflection, int a_inputParameterCount)
		{
			// Loop over all elements
			for (int i = 0; i < a_inputParameterCount; i++)
			{
				// Get descriptor of current parameter
				D3D12_SIGNATURE_PARAMETER_DESC d3d12SignatureParameterDesc;
				_reflection->GetInputParameterDesc(i, &d3d12SignatureParameterDesc);

				// Get parameter Name
				size_t strLen = strnlen(d3d12SignatureParameterDesc.SemanticName, 255);
				char* semanticName = new char[strLen + 1];
				strcpy_s(semanticName, strLen + 1, d3d12SignatureParameterDesc.SemanticName);

				// Setup InputLayout Element
				InputLayoutElement     element = {};
				element.Name		 = semanticName;
				element.Format		 = GetInputLayoutElementFromatDXGI(d3d12SignatureParameterDesc);
				element.Size		 = GetInputLayoutElementFormatSize(element.Format);
				element.ElementCount = GetInputLayoutElementFormatElementCount(element.Format);
				element.Normalized	 = false; // TODO:: check this

				// Add Element
				_inputLayout->AddElement(element);

				// Cleanup
				delete semanticName;
			}
		}
	}
}