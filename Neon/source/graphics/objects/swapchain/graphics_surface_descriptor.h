#pragma once
#include <string>

namespace Neon
{
	namespace Core { class Window; }
	namespace Graphics
	{
		class GraphicsSurfaceDescriptor
		{
		public:
			GraphicsSurfaceDescriptor() = default;

			std::string   Name;
			Core::Window* Window;
		};
	}
}