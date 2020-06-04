#include "./graphics/api/directx11/resources/shader/dx11_shader.h"
#include "./graphics/api/directx11/pipeline/inputLayout/dx11_input_layout_element_format.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"


#include "./utilities/string_utils.h"

namespace Neon
{
	namespace Graphics
	{
		DX11Shader::DX11Shader(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor)
			: Shader(_shaderDescriptor)
		{
			// For error checking
			ID3D10Blob* errorMessage = 0;

			// Compile the shaders
			DX11_ThrowIfFailed(D3DCompileFromFile(StringToWString(_shaderDescriptor->VertexShaderPath).c_str(), NULL, NULL, _shaderDescriptor->VertexShaderFunctionName.c_str(), 
				"vs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_VertexShaderBuffer, &errorMessage));
			DX11_ThrowIfFailed(D3DCompileFromFile(StringToWString(_shaderDescriptor->FragmentShaderPath).c_str(), NULL, NULL, _shaderDescriptor->FragmentShaderFunctionName.c_str(), 
				"ps_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_FragmentShaderBuffer, &errorMessage));

			// Create shaders from the buffer.
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateVertexShader(m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), NULL, &m_VertexShader));
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreatePixelShader(m_FragmentShaderBuffer->GetBufferPointer(), m_FragmentShaderBuffer->GetBufferSize(), NULL, &m_FragmentShader));


			// Shader reflection - Vertex Shader
			ID3D11ShaderReflection* vertexShaderReflection;
			DX11_ThrowIfFailed(D3DReflect(m_VertexShaderBuffer->GetBufferPointer(), m_VertexShaderBuffer->GetBufferSize(), IID_PPV_ARGS(&vertexShaderReflection)));

			D3D11_SHADER_DESC vertexShaderDesc;
			DX11_ThrowIfFailed(vertexShaderReflection->GetDesc(&vertexShaderDesc));

			// Get input layout from vertex shader
			ReflectInputLayout(&_shaderReflection.Layout, vertexShaderReflection, vertexShaderDesc.InputParameters);
		}

		DX11Shader::~DX11Shader()
		{

		}

		void DX11Shader::ReflectInputLayout(InputLayout* _inputLayout, ID3D11ShaderReflection* _reflection, int _inputParameterCount)
		{
			for (int i = 0; i < _inputParameterCount; ++i)
			{
				// Get descriptor of current parameter
				D3D11_SIGNATURE_PARAMETER_DESC d3d11SignatureParameterDesc;
				_reflection->GetInputParameterDesc(i, &d3d11SignatureParameterDesc);

				// Get parameter Name
				size_t strLen = strnlen(d3d11SignatureParameterDesc.SemanticName, 255);
				char* semanticName = new char[strLen + 1];
				strcpy_s(semanticName, strLen + 1, d3d11SignatureParameterDesc.SemanticName);

				// Setup InputLayout Element
				InputLayoutElement element = {};
				element.Name			= semanticName;
				element.Format			= GetInputLayoutElementFromatDXGI(d3d11SignatureParameterDesc);
				element.Size			= GetInputLayoutElementFormatSize(element.Format);
				element.ElementCount	= GetInputLayoutElementFormatElementCount(element.Format);
				element.Normalized		= false; // TODO:: check this

				// Add Element
				_inputLayout->AddElement(element);

				// Cleanup
				delete semanticName;
			}
		}
	}
}