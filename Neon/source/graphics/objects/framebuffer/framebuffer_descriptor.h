#pragma once

#include <string>
namespace Neon
{
	namespace Graphics
	{
		class FramebufferDescriptor
		{
			std::string Name;

			int Width;
			int Height;
			uint32_t AttachmentCount;
		};
	}
}