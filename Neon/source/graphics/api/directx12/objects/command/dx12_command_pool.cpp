#include "./graphics/api/directx12/objects/command/dx12_command_pool.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"

namespace Neon
{
	namespace Graphics
	{
		// TODOO:: abstract type
		DX12CommandPool::DX12CommandPool(CommandPoolDescriptor* _commandPoolDescriptor)
			: CommandPool(_commandPoolDescriptor)
		{
			DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator));
		}

		DX12CommandPool::~DX12CommandPool()
		{
			m_CommandAllocator->Release();
		}

		void DX12CommandPool::Reset() const
		{
			m_CommandAllocator->Reset();
		}
	}
}