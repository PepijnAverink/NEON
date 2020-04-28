#pragma once
#include "./graphics/objects/command/command_pool.h"
#include "./graphics/objects/command/command_buffer_type.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandBufferDescriptor class
		// Serves as a descriptor for the CommandBuffer class and is used on it's creation.
		// The purpose of the name is purely debugging and the Type has a default fallback so the user only has to specify the CommandPool object.
		// ------------------------------------------
		class CommandBufferDescriptor
		{
		public:
			CommandBufferDescriptor() = default;

			std::string		  Name;
			CommandBufferType Type		  = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
			CommandPool*      CommandPool = nullptr;
		};
	}
}