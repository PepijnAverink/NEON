#include "./graphics/api/vulkan/resources/shader/vk_shader.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./core/system/file_system.h"

namespace Neon
{
	namespace Graphics
	{
		VKShader::VKShader(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor)
			: Shader(_shaderDescriptor)
		{
			// Read in VertexShader file
			void* vertexShaderBuffer;
			size_t vertexShaderBufferSize;
			Core::FileSystem::ReadFile(_shaderDescriptor->VertexShaderPath, &vertexShaderBuffer, vertexShaderBufferSize);

			// Setup Creation info
			VkShaderModuleCreateInfo VCreateInfo{};
			VCreateInfo.sType	 = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			VCreateInfo.codeSize = vertexShaderBufferSize;
			VCreateInfo.pCode	 = reinterpret_cast<const uint32_t*>(vertexShaderBuffer);

			// Create VertexModule
			VK_ThrowIfFailed(vkCreateShaderModule(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &VCreateInfo, nullptr, &m_VertexShaderModule));
			
			// Setup VertexStageInfo
			m_VertexStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			m_VertexStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
			m_VertexStageInfo.module = m_VertexShaderModule;
			m_VertexStageInfo.pName  = _shaderDescriptor->VertexShaderFunctionName.c_str();


			// Read in FragmentShader file
			void* fragmenShaderBuffer;
			size_t fragmentShaderBufferSize;
			Core::FileSystem::ReadFile(_shaderDescriptor->FragmentShaderPath, &fragmenShaderBuffer, fragmentShaderBufferSize);

			// Setup Creation info
			VkShaderModuleCreateInfo FCreateInfo{};
			FCreateInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			FCreateInfo.codeSize = fragmentShaderBufferSize;
			FCreateInfo.pCode    = reinterpret_cast<const uint32_t*>(fragmenShaderBuffer);

			// Create FragmentModule
			VK_ThrowIfFailed(vkCreateShaderModule(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &FCreateInfo, nullptr, &m_FragmentShaderModule));

			// Setup FragmentStageInfo
			m_FragmentStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			m_FragmentStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
			m_FragmentStageInfo.module = m_FragmentShaderModule;
			m_FragmentStageInfo.pName  = _shaderDescriptor->FragmentShaderFunctionName.c_str();
		}

		VKShader::~VKShader()
		{
			// Destroy shader modules
			vkDestroyShaderModule(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_VertexShaderModule, nullptr);
			vkDestroyShaderModule(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_FragmentShaderModule, nullptr);
		}
	}
}