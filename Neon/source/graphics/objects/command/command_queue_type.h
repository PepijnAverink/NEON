#pragma once

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandQueueType enum
		// Specifies the type and or level of a CommandQueue obbject
		// ------------------------------------------
		enum CommandQueueType
		{
			NEON_COMMAND_QUEUE_TYPE_NONE   = 0x00,
			NEON_COMMAND_QUEUE_TYPE_DIRECT = 0x01,
		};
	}
}