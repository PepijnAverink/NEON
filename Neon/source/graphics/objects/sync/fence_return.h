#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum FenceReturn
		{
			NEON_FENCE_RETURN_NONE	  = 0x00,
			NEON_FENCE_RETURN_SUCCESS = 0x01,
			NEON_FENCE_RETURN_TIMEOUT = 0x02,
			NEON_FENCE_RETURN_ERROR	  = 0x03,
		};
	}
}