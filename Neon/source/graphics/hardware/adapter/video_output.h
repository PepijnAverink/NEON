#pragma once
#include "./graphics/hardware/adapter/display_mode.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class VideoOutput
		{
		public:
			std::vector<DisplayMode> DisplayModes;
		};
	}
}