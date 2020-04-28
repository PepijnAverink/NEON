#pragma once
#include <string>

namespace Neon
{
	namespace Graphics
	{
		class FenceDescriptor
		{
		public:
			FenceDescriptor() = default;

			std::string Name;
			uint64_t    TimeOut = UINT64_MAX;

		};
	}
}