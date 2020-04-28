#pragma once

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandBufferState enum
		// Specifies the current state of a CommandBuffer object, needs to be tracked manually per API.
		// Useful for the user to query the current state.
		// ------------------------------------------
		enum CommandBufferState
		{
			NEON_COMMAND_BUFFER_STATE_NONE	     = 0x00,
			NEON_COMMAND_BUFFER_STATE_INITIAL    = 0x01,
			NEON_COMMAND_BUFFER_STATE_RECORDING  = 0x02,
			NEON_COMMAND_BUFFER_STATE_EXECUTABLE = 0x03,
			NEON_COMMAND_BUFFER_STATE_PENDING	 = 0x04,
			NEON_COMMAND_BUFFER_STATE_INVALID	 = 0x05,
		};
	}
}