#pragma once
#include "./graphics/pipeline/rasterizer/cull_face.h"
#include <iostream>
namespace Neon
{
	namespace Graphics
	{
		inline bool GetDX12FrontCounterClockwise(const CullFace _face)
		{
			switch (_face)
			{
				case CullFace::NEON_CULL_FACE_CCW: return true;
				case CullFace::NEON_CULL_FACE_CW:  return false;
			}

			printf("[ERROR] GetDX12FrontCounterClockwise() - No valid bool was found for CullFace: %i", _face);
			return true;
		}
	}
}