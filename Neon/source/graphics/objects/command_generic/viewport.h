#pragma once

namespace Neon
{
	namespace Graphics
	{
		// TODO:: Add rect initialisation
		class Viewport
		{
		public:
			Viewport() = default;
			Viewport(const float _bottomX, const float _bottomY, const float _width, const float _height)
				: BottomX(_bottomX)
				, BottomY(_bottomY)
				, Width(_width)
				, Height(_height)
			{ }

			float BottomX;
			float BottomY;
			float Width;
			float Height;

		private:
			float MinDepth = 0;
			float MaxDepth = 1;
		};
	}
}