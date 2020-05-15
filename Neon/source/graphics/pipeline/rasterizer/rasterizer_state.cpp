#include "./graphics/pipeline/rasterizer/rasterizer_state.h"

namespace Neon
{
	namespace Graphics
	{
		RasterizerState::RasterizerState(const RasterizerStateDescriptor* _rasterizerDescriptor)
			: m_CullFace(_rasterizerDescriptor->RasterizerCullFace)
			, m_CullMode(_rasterizerDescriptor->RasterizerCullMode)
			, m_LineWidth(_rasterizerDescriptor->RasterizerLineWidth)
			, m_FillMode(_rasterizerDescriptor->RasterizerFillMode)
		{ }
	}
}