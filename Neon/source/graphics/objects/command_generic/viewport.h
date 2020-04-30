#pragma once

namespace Neon
{
	namespace Graphics
	{
		// TODO:: Add rect initialisation
		class Viewport
		{
		public:
			static Viewport* Create(const float _bottomX, const float _bottomY, const float _width, const float _height);
			virtual ~Viewport() {}
		
			virtual void SetViewport(const float _bottomX, const float _bottomY, const float _width, const float _height) = 0;

			// Getters
			inline float GetBottomX() const { return m_BottomX; }
			inline float GetBottomY() const { return m_BottomY; }
			inline float GetWidth()  const { return m_Width; }
			inline float GetHeight() const { return m_Height; }

		protected:
			Viewport(const float _bottomX, const float _bottomY, const float _width, const float _height);

			float m_BottomX;
			float m_BottomY;
			float m_Width;
			float m_Height;

			float m_MinDepth = 0;
			float m_MaxDepth = 1;
		};
	}
}