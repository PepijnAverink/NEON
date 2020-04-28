#include "./graphics/api/directx12/objects/sync/dx12_fence.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/dx12_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX12Fence::DX12Fence(const FenceDescriptor* _fenceDestriptor)
			: Fence(_fenceDestriptor)
		{
			m_FenceValue = 0;
			m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			DX12_ThrowIfFailed(DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_FenceObj)));
		}

		DX12Fence::~DX12Fence()
		{
			m_FenceObj->Release();
		}

		// TODO:: implement timeout return
		FenceReturn DX12Fence::WaitForFence()
		{
			if (m_FenceObj->GetCompletedValue() < m_FenceValue)
			{
				DX12_ThrowIfFailed(m_FenceObj->SetEventOnCompletion(m_FenceValue, m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE); // TODO:: replace infinite
			}
			m_FenceValue++;
			return NEON_FENCE_RETURN_SUCCESS;
		}

		const FenceStatus DX12Fence::GetFenceStatus()
		{
			if (m_FenceObj->GetCompletedValue() < m_FenceValue)
				m_Status = FenceStatus::NEON_FENCE_STATUS_NOT_READY;
			else
				m_Status = FenceStatus::NEON_FENCE_STATUS_READY;

			return m_Status;
		}

		// TODO:: implement reset
		void DX12Fence::Reset() const
		{

		}
	}
}