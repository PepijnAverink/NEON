#pragma once
#include "./graphics/objects/command/command_queue_descriptor.h"
#include "./graphics/objects/sync/fence.h"
#include "./graphics/graphics_defines.h"

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandQueue class
		// Serves as an abstraction of a CommandQueue in respective API's
		//
		// The name is stored for debugging reasons and is not compiled in release mode, the getter will return an empty string in release mode.
		// ------------------------------------------
		class CommandBuffer;
		class CommandQueue
		{
		public:
			// TODO:: Take in device
			static CommandQueue* Create(CommandQueueDescriptor* _commandQueueDescriptor);
			virtual ~CommandQueue() {}

			virtual void ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const = 0;
			virtual void ExecuteCommandBuffers(const unsigned int _commandBufferCount, CommandBuffer* _commandBuffers[]) const = 0;

			// Getters
			inline const std::string	  GetCommandQueueName() const;
			inline const uint32_t		 GetCommandQueueIndex() const { return m_QueueIndex; }
			inline const CommandQueueType GetCommandQueueType() const { return m_Type; }

		protected:
			CommandQueue(CommandQueueDescriptor* _commandQueueDescriptor);

#if defined(NEON_DEBUG)
			std::string		 m_Name;
#endif

			uint32_t		 m_QueueIndex;
			CommandQueueType m_Type;

		};
	}
}