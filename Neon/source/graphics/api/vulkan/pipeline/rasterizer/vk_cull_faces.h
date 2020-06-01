#pragma once
#include "./graphics/pipeline/rasterizer/cull_face.h"

#include <vulkan/vulkan.h>
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// GetVKCullFace function
		// Retrieves the VkPolygonMode from the FillMode parameter passed by the user.
		// ------------------------------------------
		VkFrontFace GetVKCullFace(const CullFace _cullMode)
		{
			switch (_cullMode)
			{
				case CullFace::NEON_CULL_FACE_CW:   return VK_FRONT_FACE_CLOCKWISE;
				case CullFace::NEON_CULL_FACE_CCW:  return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			}

			// TODO:: Inspect default, maybe change it?
			printf("[warning] GetVKCullFace() - No matching CullFace found, default to: NEON_CULL_FACE_CW, CullFace: %i", _cullMode);
			return VK_FRONT_FACE_CLOCKWISE;
		}
	}
}