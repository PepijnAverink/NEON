#pragma once
#include "./graphics/objects/command_generic/topology.h"

#include <d3dcommon.h>
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		inline D3D_PRIMITIVE_TOPOLOGY GetDX12Topology(const Topology _topology)
		{
			switch (_topology)
			{
				case Topology::NEON_TOPOLOGY_POINT_LIST:	 return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
				case Topology::NEON_TOPOLOGY_LINE_LIST:		 return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
				case Topology::NEON_TOPOLOGY_LINE_STRIP:	 return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
				case Topology::NEON_TOPOLOGY_TRIANGLE_LIST:  return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				case Topology::NEON_TOPOLOGY_TRIANGLE_STRIP: return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}

			printf("[warning] GetDX12Topology() - No matching Topology found, default to: NEON_TOPOLOGY_TRIANGLE_LIST, Topology: %i", _topology);
			return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	}
}