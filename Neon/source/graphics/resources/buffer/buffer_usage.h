#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum BufferUsage
		{
			NEON_BUFFER_USAGE_NONE	  = 0x00,
			NEON_BUFFER_USAGE_STATIC  = 0x01,
			NEON_BUFFER_USAGE_DYNAMIC = 0x02,
		};
	}
}