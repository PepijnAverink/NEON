#pragma once
#include "./graphics/objects/command/command_queue.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{

		// ------------------------------------------
		// VKCommandQueue class
		// The m_CommandQueueObj object is by the logical device and cant be destroyed on its own.
		// ------------------------------------------
		class VKCommandQueue : public CommandQueue
		{
		public:
			VKCommandQueue(CommandQueueDescriptor* _commandQueueDescriptor);
			virtual ~VKCommandQueue() {}

			virtual void ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const override;

		private:
			friend class VKGraphicsContext; // TODO:: Remove me
			VkQueue m_CommandQueueObj;
		};
	}
}