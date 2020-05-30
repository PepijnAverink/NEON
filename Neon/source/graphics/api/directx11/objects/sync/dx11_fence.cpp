#include "./graphics/api/directx11/objects/sync/dx11_fence.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX11Fence::DX11Fence(const FenceDescriptor* _fenceDestriptor)
			: Fence(_fenceDestriptor)
			, m_FenceValue(0)
		{
			m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateFence(m_FenceValue, D3D11_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_FenceObj)));
		}

		DX11Fence::~DX11Fence()
		{
			m_FenceObj->Release();
		}

		FenceReturn DX11Fence::WaitForFence()
		{ 
			if (m_FenceObj->GetCompletedValue() < m_FenceValue)
			{
				DX11_ThrowIfFailed(m_FenceObj->SetEventOnCompletion(m_FenceValue, m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE); // TODO:: replace infinite
			}
			m_FenceValue++;
			return NEON_FENCE_RETURN_SUCCESS;
		}

		const FenceStatus DX11Fence::GetFenceStatus()
		{
			if (m_FenceObj->GetCompletedValue() < m_FenceValue)
				m_Status = FenceStatus::NEON_FENCE_STATUS_NOT_READY;
			else
				m_Status = FenceStatus::NEON_FENCE_STATUS_READY;

			return m_Status;
		}

		void DX11Fence::Reset() const
		{

		}
	}
}