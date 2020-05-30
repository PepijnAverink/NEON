#include "./graphics/api/directx12/objects/command/dx12_command_queue.h"
#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"
#include "./graphics/api/directx12/objects/sync/dx12_fence.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/dx12_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{

		// TODO:: abstract type
		DX12CommandQueue::DX12CommandQueue(CommandQueueDescriptor* _commandQueueDescriptor)
			: CommandQueue(_commandQueueDescriptor)
		{
			// Set up descriptor
			D3D12_COMMAND_QUEUE_DESC cqDesc = {};
			cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			cqDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
			
			// Create the queue
			DX12_ThrowIfFailed(DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&m_CommandQueueObj)));
		}

		DX12CommandQueue::~DX12CommandQueue()
		{
			m_CommandQueueObj->Release();
		}

		void DX12CommandQueue::ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const
		{
			auto commandBuffer = NEON_CAST(DX12CommandBuffer*, _commandBuffer);

			ID3D12CommandList* ppCommandLists[] = { commandBuffer->m_CommandListObj };
			m_CommandQueueObj->ExecuteCommandLists(1, ppCommandLists);

			// Signal fence
			auto fence = NEON_CAST(DX12Fence*, _signalFence);
			m_CommandQueueObj->Signal(fence->m_FenceObj, fence->m_FenceValue);
		}
	}
}