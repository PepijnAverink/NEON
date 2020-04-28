#pragma once

namespace Neon
{
	namespace Graphics
	{
		class DisplayModeDescriptor
		{
		public:
			void GetResolutionRatio(float& _ratio); 
			void GetResolutionRatio(float& _numerator, float& _denominator);

			bool operator>(const DisplayModeDescriptor& _other) const;
			friend bool operator>(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs);
			bool operator>=(const DisplayModeDescriptor& _other) const;
			friend bool operator>=(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs);

			bool operator<(const DisplayModeDescriptor& _other) const;
			friend bool operator<(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs);
			bool operator<=(const DisplayModeDescriptor& _other) const;
			friend bool operator<=(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs);

			bool operator==(const DisplayModeDescriptor& _other) const;
			friend bool operator==(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs);

			bool operator!=(const DisplayModeDescriptor& _other) const;
			friend bool operator!=(const DisplayModeDescriptor& _lhs, const DisplayModeDescriptor& _rhs);

			int ResolutionWidth;
			int ResolutionHeight;

			unsigned int RefreshRate;
		};
	}
}