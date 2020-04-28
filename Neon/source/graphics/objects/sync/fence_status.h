#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum FenceStatus
		{
			NEON_FENCE_STATUS_NONE		= 0x00,
			NEON_FENCE_STATUS_READY		= 0x01,
			NEON_FENCE_STATUS_NOT_READY = 0x02,
		};
	}
}