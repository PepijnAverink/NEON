#include "./graphics/graphics_driver.h"
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		static GraphicsDriver* s_GraphicsDriver = nullptr;

		void GraphicsDriver::Create(const GraphicsAPI _api)
		{
			if (s_GraphicsDriver != nullptr)
			{
				printf("[WARNING] GraphicsDriver::Create() - User called Create, but a valid instance of GraphicsDriver was already found.\n\t Not overwritten.");
				return;
			}

			s_GraphicsDriver = new GraphicsDriver(_api);
		}

		void Graphics::GraphicsDriver::Destroy()
		{
			if (s_GraphicsDriver == nullptr)
			{
				printf("[WARNING] GraphicsDriver::Destroy() - User called Destroy, but no valid instance of GraphicsDriver was found.");
				return;
			}

			delete s_GraphicsDriver; s_GraphicsDriver = nullptr;
		}

		GraphicsAPI GraphicsDriver::GetGraphicsAPI()
		{
			return s_GraphicsDriver->m_API;
		}

		GraphicsDriver::GraphicsDriver(const GraphicsAPI _api)
		{
			m_API = _api;
		}
	}
}