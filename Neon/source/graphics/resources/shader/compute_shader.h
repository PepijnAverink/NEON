#pragma once
#include "./graphics/resources/shader/compute_shader_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class ComputeShader
		{
		public:
			static ComputeShader* Create(const ComputeShaderDescriptor* _computeShaderDescriptor);
			virtual ~ComputeShader() {}

		protected:
			ComputeShader(const ComputeShaderDescriptor* _computeShaderDescriptor);

			std::string m_ComputeShaderPath;
			std::string m_ComputeFunctionName;

			bool		m_HotReload;
		};
	}
}