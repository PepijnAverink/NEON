#pragma once
#include "./graphics/objects/sync/fence.h"

#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		class DX12Fence : public Fence
		{
		public:
			DX12Fence(const FenceDescriptor* _fenceDestriptor);
			virtual ~DX12Fence();

			virtual FenceReturn         WaitForFence() override;
			virtual const FenceStatus GetFenceStatus() override;

			virtual void Reset() const override;

		private:
			friend class DX12CommandQueue;
			friend class DX12GraphicsContext; // TODO:: Remove me
			ID3D12Fence* m_FenceObj;
			uint64_t     m_FenceValue;

			HANDLE		 m_FenceEvent;
		};
	}
}