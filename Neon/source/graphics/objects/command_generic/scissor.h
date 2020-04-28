#pragma once

namespace Neon
{
	namespace Graphics
	{
		class Scissor
		{
		public:
			Scissor() = default;
			Scissor(const float _bottomX, const float _bottomY, const float _width, const float _height)
				: BottomX(_bottomX)
				, BottomY(_bottomY)
				, Width(_width)
				, Height(_height)
			{ }

			float BottomX;
			float BottomY;
			float Width;
			float Height;
		};
	}
}