#pragma once
#include "./graphics/hardware/adapter/video_output.h"
#include "./graphics/hardware/adapter/video_adapter_type.h"

#include <string>
#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class VideoAdapter
		{
		public:
			std::string				 Name;
			std::string				 Vendor;
			VideoAdapterType		 Type;

			uint64_t				 VideoMemory;
			std::vector<DisplayMode> DisplayModes;
		};
	}
}