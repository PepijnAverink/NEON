#pragma once

namespace Neon
{
	namespace Graphics
	{
		class DisplayMode
		{
		public:
			void GetResolutionRatio(float& _ratio); 
			void GetResolutionRatio(float& _numerator, float& _denominator);

			int ResolutionWidth;
			int ResolutionHeight;

			unsigned int Numerator;
			unsigned int Denominator;
		};
	}
}