#include "./graphics/pipeline/rasterizer/rasterizer_state.h"

namespace Neon
{
	namespace Graphics
	{
		RasterizerState::RasterizerState(const RasterizerStateDescriptor* _rasterizerDescriptor)
		{ 
			if (_rasterizerDescriptor != nullptr)
			{
				m_CullFace = _rasterizerDescriptor->CullFace;
				m_CullMode = _rasterizerDescriptor->CullMode;
				m_FillMode = _rasterizerDescriptor->FillMode;
				m_LineWidth = _rasterizerDescriptor->LineWidth;
			}
		}
	}
}