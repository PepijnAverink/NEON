#pragma once
#include "./graphics/pipeline/rasterizer/rasterizer_state_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class RasterizerState
		{
		public:
			RasterizerState(const RasterizerStateDescriptor* _rasterizerDescriptor);

		private:
			CullFace m_CullFace;
			CullMode m_CullMode;

			float	 m_LineWidth;
			FillMode m_FillMode;
		};
	}
}