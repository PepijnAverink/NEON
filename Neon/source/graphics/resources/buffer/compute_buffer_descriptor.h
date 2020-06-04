#pragma once
#include <string>

namespace Neon
{
	namespace Graphics
	{
		class ComputeBufferDescriptor
		{
		public:
			ComputeBufferDescriptor() = default;

			std::string Name;
			uint32_t	Size;
		};
	}
}