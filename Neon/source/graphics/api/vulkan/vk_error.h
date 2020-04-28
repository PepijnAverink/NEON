#pragma once
#include <vulkan/vulkan.h>
#include <exception>

namespace Neon
{
	namespace Graphics
	{
		inline void VK_ThrowIfFailed(VkResult _result)
		{
			if (_result != VK_SUCCESS)
			{
				throw std::exception();
			}
		}
	}
}