#pragma once
#include "./graphics/hardware/adapter/video_output_descriptor.h"
#include "./graphics/hardware/adapter/adapter_type.h"

#include <string>
#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class VideoAdapterDescriptor
		{
		public:
			std::string Name;
			std::string Vendor;
			AdapterType Type;

			uint64_t    VideoMemory;
			std::vector<VideoOutputDescriptor> Outputs;
		};
	}
}