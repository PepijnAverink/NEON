#pragma once
#include "./graphics/objects/command/command_pool.h"

#include <d3d12.h>
#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class DX12CommandPool final : public CommandPool
		{
		public:
			DX12CommandPool(CommandPoolDescriptor* _commandPoolDescriptor);
			virtual ~DX12CommandPool();

			virtual void Reset() const override;

		private:
			friend class DX12CommandBuffer;
			friend class DX12GraphicsContext; // Remove me
			ID3D12CommandAllocator* m_CommandAllocator;
		};
	}
}