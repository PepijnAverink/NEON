#include "./graphics/api/directx11/resources/shader/dx11_compute_shader.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./utilities/string_utils.h"

namespace Neon
{
	namespace Graphics
	{
		DX11ComputeShader::DX11ComputeShader(const ComputeShaderDescriptor* _computeShaderDescriptor)
			: ComputeShader(_computeShaderDescriptor)
		{
			// For error checking
			ID3D10Blob* errorMessage = 0;

			// Compile the Compute shader
			HRESULT hr = D3DCompileFromFile(StringToWString(_computeShaderDescriptor->ComputeShaderPath).c_str(), NULL, NULL, 
				_computeShaderDescriptor->ComputeShaderFunctionName.c_str(), "cs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &m_ComputeShaderBuffer, &errorMessage);

			if (FAILED(hr))
				printf("[ERROR] DX11ComputeShader - An error occured while compiling the ComputeShader\n%s", errorMessage->GetBufferPointer());

			// Create Compute Shader
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateComputeShader(m_ComputeShaderBuffer->GetBufferPointer(), m_ComputeShaderBuffer->GetBufferSize(), NULL, &m_ComputeShader));
		}

		DX11ComputeShader::~DX11ComputeShader()
		{

		}
	}
}