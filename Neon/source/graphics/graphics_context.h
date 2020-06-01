#pragma once
#include "./graphics/graphics_context_descriptor.h"
#include "./graphics/hardware/adapter/video_adapter.h"

#include <string>
#include <vector>

namespace Neon
{
	const int frameBufferCount = 3;

	namespace Core { class Window; }
	namespace Graphics
	{
		// TODO:: Only do initialisation code, pass the window to swapchain
		// Graphics context will then live in general and serve as a container of abstracted objects
		class GraphicsContext
		{
		public:
			static GraphicsContext* Create(const GraphicsContextDescriptor* _graphicsContextDescriptor);
			static GraphicsContext* GetGraphicsContext();
			virtual ~GraphicsContext();

			virtual bool Initialize() = 0;
			virtual bool Terminate()  = 0;

		protected:
			GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor);

			unsigned int m_ClientWidth;
			unsigned int m_ClientHeight;

			unsigned int m_ScreenWidth;
			unsigned int m_ScreenHeight;

			bool		 m_VSync = false;

			// Video card
			std::vector<VideoAdapter*>  m_Adapters;
			uint32_t					m_CurrentAdapter;
		};
	}
}