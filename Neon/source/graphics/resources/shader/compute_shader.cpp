#include "./graphics/resources/shader/compute_shader.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/resources/shader/dx11_compute_shader.h"
#endif


//#if defined (NEON_SUPPORT_DIRECTX12)	
//#include "./graphics/api/directx12/resources/shader/dx12_shader.h"
//#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/resources/shader/vk_shader.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		ComputeShader* ComputeShader::Create(const ComputeShaderDescriptor* _computeShaderDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

		//	// Vulkan
		//	if (api == GraphicsAPI::VULKAN)
		//		return new VKShader(_shaderReflection, _shaderDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11ComputeShader(_computeShaderDescriptor);

		//	// DirectX12
		//	if (api == GraphicsAPI::DIRECTX12)
		//		return new DX12Shader(_shaderReflection, _shaderDescriptor);

			printf("[ERROR] ComputeShader::Create() No valid api abstraction for found for object: ComputeShader");
			return nullptr;
		}

		ComputeShader::ComputeShader(const ComputeShaderDescriptor* _computeShaderDescriptor)
			: m_ComputeShaderPath(_computeShaderDescriptor->ComputeShaderPath)
			, m_ComputeFunctionName(_computeShaderDescriptor->ComputeShaderFunctionName)
			, m_HotReload(_computeShaderDescriptor->HotReload)
		{ }
	}
}