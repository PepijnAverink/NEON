#pragma once
#include <string>

namespace Neon
{
	namespace Graphics
	{
		class Texture2DDescriptor
		{
		public:
			Texture2DDescriptor() = default;

			std::string Name;

			unsigned int Width;
			unsigned int Height;

			unsigned int Color;
		};
	}
}