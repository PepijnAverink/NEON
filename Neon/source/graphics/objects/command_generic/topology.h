#pragma once

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// Topology enum
		// An enum describing the topology for the graphics pipeline.
		// ------------------------------------------
		enum Topology
		{
			NEON_TOPOLOGY_NONE			 = 0x00,
			NEON_TOPOLOGY_POINT_LIST	 = 0x01,
			NEON_TOPOLOGY_LINE_LIST		 = 0x02,
			NEON_TOPOLOGY_LINE_STRIP	 = 0x03,
			NEON_TOPOLOGY_TRIANGLE_LIST	 = 0x04,
			NEON_TOPOLOGY_TRIANGLE_STRIP = 0x05,
		};
	}
}