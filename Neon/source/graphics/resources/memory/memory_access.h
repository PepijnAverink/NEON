#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum MemoryAccess
		{
			NEON_MEMORY_ACCESS_NONE			= 0x00,
			NEON_MEMORY_ACCESS_GPU_ONLY		= 0x01,
			NEON_MEMORY_ACCESS_GPU_AND_CPU  = 0x02,
		};
	}
}