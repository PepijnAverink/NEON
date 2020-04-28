#pragma once
#include "./graphics/objects/command/command_pool_descriptor.h"
#include "./graphics/graphics_defines.h"

#include <stdint.h>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandPool class
		// Serves as a pool for command recorded on a certain CommandBuffer object.
		// A single CommandPool can hold multiple CommanBuffers, but only one can be recorded at a time.
		//
		// The CommandPool can only be used to create CommandBuffers of the same type.
		// This object also keeps track of the amount of CommanBuffer objects attached to this CommandPool.
		// The name is stored for debugging reasons and is not compiled in release mode, the getter will return an empty string in release mode.
		//
		// Another note worthy thing is that the CommandPool will take ownership of the CommandBuffer and cannot be properly deleted without deleting the CommandPool it was created with.
		// ------------------------------------------
		class CommandBuffer;
		class CommandPool
		{
		public:
			static CommandPool* Create(CommandPoolDescriptor* _commandPoolDescriptor);
			virtual ~CommandPool() {}

			uint32_t AddCommandBuffer(const CommandBuffer* _commandBuffer);

			// Reset the entire CommandPool object (and all CommandBuffers using this CommandPool)
			virtual void Reset() const = 0;

			// Getters
			const std::string     GetCommandBufferName() const;
			const uint32_t       GetCommandBufferCount() const { return m_CommandBufferCount; }
			const CommandBufferType GetCommandPoolType() const { return m_Type; }

		protected:
			CommandPool(CommandPoolDescriptor* _commandPoolDescriptor);

#if defined(NEON_DEBUG)
			std::string		   m_Name;
#endif
			uint32_t		  m_CommandBufferCount = 0;
			CommandBufferType m_Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_NONE;
		};
	}
}