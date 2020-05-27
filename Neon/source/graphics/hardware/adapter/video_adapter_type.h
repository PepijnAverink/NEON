#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum VideoAdapterType
		{
			NEON_VIDEO_ADAPTER_TYPE_NONE     = 0x00,
			NEON_VIDEO_ADAPTER_TYPE_UNKNOWN  = 0x01,
			NEON_VIDEO_ADAPTER_TYPE_SOFTWARE = 0x02,
			NEON_VIDEO_ADAPTER_TYPE_HARDWARE = 0x03,
		};
	}
}