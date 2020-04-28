#pragma once
#include "./graphics/pipeline/rasterizer/cull_modes.h"

#include <vulkan/vulkan.h>
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// GetVKCullMode function
		// Retrieves the VkCullModeFlagBits from the CullMode parameter passed by the user.
		// ------------------------------------------
		VkCullModeFlagBits GetVKCullMode(const CullMode _cullMode)
		{
			switch (_cullMode)
			{
				case CullMode::NEON_CULL_MODE_FRONT: return VK_CULL_MODE_FRONT_BIT;
				case CullMode::NEON_CULL_MODE_BACK:  return VK_CULL_MODE_BACK_BIT;
				case CullMode::NEON_CULL_MODE_FRONT_AND_BACK:  return VK_CULL_MODE_FRONT_AND_BACK;
			}

			printf("[warning] GetVKCullMode() - No matching CullMode found, default to: NEON_CULL_MODE_FRONT, CullMode: %i", _cullMode);
			return VK_CULL_MODE_FRONT_BIT;
		}
	}
}