#pragma once
#include "./graphics/objects/sync/fence.h"

#include <d3d11.h>
#include <d3d11_3.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11Fence : public Fence
		{
		public:
			DX11Fence(const FenceDescriptor* _fenceDestriptor);
			virtual ~DX11Fence();

			virtual FenceReturn         WaitForFence() override;
			virtual const FenceStatus GetFenceStatus() override;

			virtual void Reset() const override;

		private:
			friend class DX11Swapchain;
			ID3D11Fence* m_FenceObj;
			uint64_t     m_FenceValue;

			HANDLE		 m_FenceEvent;
		};
	}
}