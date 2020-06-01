#pragma once
#include "./graphics/objects/command/command_queue.h"

#include <D3D11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11CommandQueue : public CommandQueue
		{
		public:
			DX11CommandQueue(CommandQueueDescriptor* _commandQueueDescriptor);
			virtual ~DX11CommandQueue();

			virtual void ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const override;
			
		private:
			
		};
	}
}