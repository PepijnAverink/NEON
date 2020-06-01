#include "./graphics/resources/shader/shader.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/resources/shader/dx11_shader.h"
#endif


#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/resources/shader/dx12_shader.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/resources/shader/vk_shader.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		Shader* Shader::Create(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKShader(_shaderReflection, _shaderDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11Shader(_shaderReflection, _shaderDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12Shader(_shaderReflection, _shaderDescriptor);

			printf("[ERROR] Shader::Create() No valid api abstraction for found for object: Shader");
			return nullptr;
		}

		Shader::Shader(const ShaderDescriptor* _shaderDescriptor)
			: m_VertexShaderPath(_shaderDescriptor->VertexShaderPath)
			, m_VertexFunctionName(_shaderDescriptor->VertexShaderFunctionName)
			, m_FragmentShaderPath(_shaderDescriptor->FragmentShaderPath)
			, m_FragmentFunctionName(_shaderDescriptor->FragmentShaderFunctionName)
			, m_HotReload(_shaderDescriptor->HotReload)
		{ }
	}
}