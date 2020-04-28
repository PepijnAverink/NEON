#pragma once
#include "./graphics/objects/command/command_buffer_type.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandPoolDescriptor class
		// Serves as a descriptor for the CommandPool class and is used on it's creation.
		// The purpose of the name is purely debugging and the Type has a default fallback so the user does not have to specify anything for a default CommandPool object.
		// ------------------------------------------
		class CommandPoolDescriptor
		{
		public:
			std::string		  Name;
			CommandBufferType Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
		};
	}
}