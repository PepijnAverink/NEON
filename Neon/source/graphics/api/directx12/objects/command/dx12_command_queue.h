#pragma once
#include "./graphics/objects/command/command_queue.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12CommandQueue final : public CommandQueue
		{
		public:
			DX12CommandQueue(CommandQueueDescriptor* _commandQueueDescriptor);
			virtual ~DX12CommandQueue();

			virtual void ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const override;

		private:
			friend class DX12Swapchain;
			friend class DX12GraphicsContext; // Remove me
			ID3D12CommandQueue* m_CommandQueueObj;
		};
	}
}