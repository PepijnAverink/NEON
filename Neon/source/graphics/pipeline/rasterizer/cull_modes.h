#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum CullMode
		{
			NEON_CULL_MODE_NONE				= 0x00,
			NEON_CULL_MODE_FRONT			= 0x01,
			NEON_CULL_MODE_BACK				= 0x02,
			NEON_CULL_MODE_FRONT_AND_BACK	= 0x03, // TODO:: Verify support for API's
		};
	}
}