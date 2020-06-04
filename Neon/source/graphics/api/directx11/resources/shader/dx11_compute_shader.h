#pragma once
#include "./graphics/resources/shader/compute_shader.h"

#include <D3D11.h>
#include <D3DCompiler.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11ComputeShader : public ComputeShader
		{
		public:
			DX11ComputeShader(const ComputeShaderDescriptor* _computeShaderDescriptor);
			virtual ~DX11ComputeShader();

		private:
			friend class DX11CommandBuffer;
			ID3D11ComputeShader* m_ComputeShader;
			ID3D10Blob*			 m_ComputeShaderBuffer = 0;
		};
	}
}