#pragma once
#include "./graphics/graphics_defines.h"

namespace Neon
{
	namespace Graphics
	{
		enum GraphicsAPI
		{
			NONE,
			OPENGL,
			DIRECTX11,
			DIRECTX12,
			VULKAN,
		};

		class GraphicsDriver
		{
		public:
			static void Create(const GraphicsAPI _api);
			static void Destroy();

			static GraphicsAPI GetGraphicsAPI();

		private: 
			GraphicsDriver(const GraphicsAPI _api);

			GraphicsAPI m_API;
		};
	}
}