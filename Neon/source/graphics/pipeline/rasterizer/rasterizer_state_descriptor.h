#pragma once
#include "./graphics/pipeline/rasterizer/cull_face.h"
#include "./graphics/pipeline/rasterizer/cull_mode.h"
#include "./graphics/pipeline/rasterizer/fill_mode.h"

namespace Neon
{
	namespace Graphics
	{
		class RasterizerStateDescriptor
		{
		public:
			RasterizerStateDescriptor() = default;

			CullFace CullFace;	// TODO:: Set default
			CullMode CullMode  = CullMode::NEON_CULL_MODE_NONE;

			float	 LineWidth = 1.0f;
			FillMode FillMode  = FillMode::NEON_FILL_MODE_FILL;
		};
	}
}