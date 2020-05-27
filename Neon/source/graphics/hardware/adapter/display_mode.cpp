#include "./graphics/hardware/adapter/display_mode.h"


#include <numeric>
#include <algorithm>

namespace Neon
{
	namespace Graphics
	{
		void DisplayMode::GetResolutionRatio(float& _ratio)
		{
			_ratio = ResolutionWidth / ResolutionHeight;
		}

		// TODO:: implement
		void DisplayMode::GetResolutionRatio(float& _numerator, float& _denominator)
		{
			
		}
	}
}