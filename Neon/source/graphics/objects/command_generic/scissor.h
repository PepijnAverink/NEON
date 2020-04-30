#pragma once

namespace Neon
{
	namespace Graphics
	{
		class Scissor
		{
		public:
			static Scissor* Create(const float _bottomX, const float _bottomY, const float _width, const float _height);
			virtual ~Scissor() {}

			virtual void SetScissor(const float _bottomX, const float _bottomY, const float _width, const float _height) = 0;

		protected:
			Scissor(const float _bottomX, const float _bottomY, const float _width, const float _height);

			float m_BottomX;
			float m_BottomY;
			float m_Width;
			float m_Height;
		};
	}
}