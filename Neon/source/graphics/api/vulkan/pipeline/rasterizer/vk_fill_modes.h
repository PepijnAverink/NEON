#pragma once
#include "./graphics/pipeline/rasterizer/fill_mode.h"

#include <vulkan/vulkan.h>
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// GetVKFillMode function
		// Retrieves the VkPolygonMode from the FillMode parameter passed by the user.
		// ------------------------------------------
		VkPolygonMode GetVKFillMode(const FillMode _fillMode)
		{
			switch (_fillMode)
			{
				case FillMode::NEON_FILL_MODE_LINE:  return VK_POLYGON_MODE_LINE;
				case FillMode::NEON_FILL_MODE_FILL:  return VK_POLYGON_MODE_FILL;
			}

			printf("[warning] GetVKFillMode() - No matching FillMode found, default to: NEON_FILL_MODE_FILL, FillMode: %i", _fillMode);
			return VK_POLYGON_MODE_FILL;
		}
	}
}