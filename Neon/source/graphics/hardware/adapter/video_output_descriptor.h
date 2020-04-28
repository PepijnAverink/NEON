#pragma once
#include "./graphics/hardware/adapter/display_mode_descriptor.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class VideoOutputDescriptor
		{
		public:
			std::vector<DisplayModeDescriptor> DisplayModes;
		};
	}
}