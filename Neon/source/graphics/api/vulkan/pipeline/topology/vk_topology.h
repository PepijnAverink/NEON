#pragma once
#include "./graphics/objects/command_generic/topology.h"

#include <vulkan/vulkan.h>
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// GetVKTopology function
		// Retrieves the VkPrimitiveTopology from the Topology parameter passed by the user.
		// ------------------------------------------
		VkPrimitiveTopology GetVKTopology(const Topology _topology)
		{
			switch (_topology)
			{
				case Topology::NEON_TOPOLOGY_POINT_LIST:	 return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
				case Topology::NEON_TOPOLOGY_LINE_LIST:		 return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
				case Topology::NEON_TOPOLOGY_LINE_STRIP:	 return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
				case Topology::NEON_TOPOLOGY_TRIANGLE_LIST:  return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
				case Topology::NEON_TOPOLOGY_TRIANGLE_STRIP: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			}

			printf("[warning] GetVKTopology() - No matching Topology found, default to: NEON_TOPOLOGY_TRIANGLE_LIST, Topology: %i", _topology);
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}
}