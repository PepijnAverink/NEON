#pragma once
#include "./graphics/resources/shader/shader.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKShader : public Shader
		{
		public:
			VKShader(ShaderReflection& _shaderReflection, const ShaderDescriptor* _shaderDescriptor);
			virtual ~VKShader();

		private:
			VkShaderModule m_VertexShaderModule;
			VkShaderModule m_FragmentShaderModule;

			friend class VKGraphicsContext; // Remove me
			friend class VKGraphicsPipeline;
			VkPipelineShaderStageCreateInfo m_VertexStageInfo;
			VkPipelineShaderStageCreateInfo m_FragmentStageInfo;
		};
	}
}