#pragma once

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandBufferType enum
		// Specifies the type and or level of a CommandBuffer obbject
		// ------------------------------------------
		enum CommandBufferType
		{
			NEON_COMMAND_BUFFER_TYPE_NONE	  = 0x00,
			NEON_COMMAND_BUFFER_TYPE_DIRECT   = 0x01,
			NEON_COMMAND_BUFFER_TYPE_INDIRECT = 0x02,
			NEON_COMMAND_BUFFER_TYPE_COMPUTE  = 0x03,
		};
	}
}