#pragma once
#include <string>

namespace Neon
{
	const int frameBufferCount = 3;

	namespace Core { class Window; }
	namespace Graphics
	{
		// TODO:: Only do initialisation code, pass the window to swapchain
		// TODO:: Graphics context becomes obsolete, move API init to A graphics device abstraction
		// Graphics context will then live in general and serve as a container of abstracted objects
		class GraphicsContext
		{
		public:
			static GraphicsContext* Create(Core::Window* _window);
			static GraphicsContext* GetGraphicsContext();
			virtual ~GraphicsContext() {};

			virtual bool Initialize() = 0;
			virtual bool Terminate() = 0;

			virtual void Present() = 0;

		protected:
			GraphicsContext(Core::Window* _window);

			unsigned int m_ClientWidth;
			unsigned int m_ClientHeight;

			bool		 m_VSync = false;

			// Video card
			std::string  m_VideoCardName;
			unsigned int m_VideoCardMemory = 0;
		};
	}
}