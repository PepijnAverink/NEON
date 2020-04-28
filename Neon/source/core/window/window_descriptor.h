#pragma once
#include <string>

namespace Neon
{
	namespace Core
	{
		struct WindowDescriptor
		{
			std::string Title;
			int Width, Height;

			bool VSync;
		};
	}
}