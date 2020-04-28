#include "./graphics/hardware/adapter/display_mode_descriptor.h"


#include <numeric>
#include <algorithm>

namespace Neon
{
	namespace Graphics
	{
		void DisplayModeDescriptor::GetResolutionRatio(float& _ratio)
		{
			_ratio = ResolutionWidth / ResolutionHeight;
		}

		// TODO:: implement
		void DisplayModeDescriptor::GetResolutionRatio(float& _numerator, float& _denominator)
		{
			
		}

		bool DisplayModeDescriptor::operator==(const DisplayModeDescriptor& _other) const
		{
			return (ResolutionWidth == _other.ResolutionWidth) && (ResolutionHeight == _other.ResolutionHeight) && (RefreshRate == _other.RefreshRate);
		}

		bool DisplayModeDescriptor::operator!=(const DisplayModeDescriptor& _other) const
		{
			return (ResolutionWidth != _other.ResolutionWidth) || (ResolutionHeight != _other.ResolutionHeight) || (RefreshRate != _other.RefreshRate);
		}

		bool operator==(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs)
		{
			return (_lhs.ResolutionWidth == _rhs.ResolutionWidth) && (_lhs.ResolutionHeight == _rhs.ResolutionHeight) && (_lhs.RefreshRate == _rhs.RefreshRate);
		}

		bool operator!=(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs)
		{
			return (_lhs.ResolutionWidth != _rhs.ResolutionWidth) || (_lhs.ResolutionHeight != _rhs.ResolutionHeight) || (_lhs.RefreshRate != _rhs.RefreshRate);
		}
	}
}