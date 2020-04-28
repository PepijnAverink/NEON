#pragma once
#include "./graphics/pipeline/rasterizer/cull_faces.h"
#include "./graphics/pipeline/rasterizer/cull_modes.h"
#include "./graphics/pipeline/rasterizer/fill_modes.h"

namespace Neon
{
	namespace Graphics
	{
		class RasterizerState
		{
		public:
			RasterizerState() = default;

			CullFace RasterizerCullFace;	// TODO:: Set default
			CullMode RasterizerCullMode		= CullMode::NEON_CULL_MODE_NONE;

			float	 RasterizerLineWidth	= 1.0f;
			FillMode RasterizerFillMode		= FillMode::NEON_FILL_MODE_FILL;
		};
	}
}