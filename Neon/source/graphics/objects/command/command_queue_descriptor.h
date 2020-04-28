#pragma once
#include "./graphics/objects/command/command_queue_type.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandQueueDescriptor class
		// Serves as a descriptor for the CommandQueue class and is used on it's creation.
		// The purpose of the name is purely debugging and the Type has a default fallback, the queueIndex is adviced to be set by the user.
		// ------------------------------------------
		class CommandQueueDescriptor
		{
		public:
			CommandQueueDescriptor() = default;

			std::string		 Name;
			uint32_t		 QueueIndex = 0;
			CommandQueueType Type = CommandQueueType::NEON_COMMAND_QUEUE_TYPE_DIRECT;
		};
	}
}