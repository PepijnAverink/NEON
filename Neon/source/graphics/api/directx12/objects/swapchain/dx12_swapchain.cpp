#include "./graphics/api/directx12/objects/swapchain/dx12_swapchain.h"
#include "./graphics/api/directx12/objects/swapchain/dx12_graphics_surface.h"
#include "./graphics/api/directx12/objects/command/dx12_command_queue.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#include "./graphics/api/directx12/objects/sync/dx12_fence.h"
#include "./graphics/api/directx12/dx12_error.h"


#include "./utilities/casting/casting_helper.h"
#include "./core/window/window.h"

namespace Neon
{
	namespace Graphics
	{
		DX12Swapchain::DX12Swapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor)
			: Swapchain(_swapchainDescriptor)
		{
			// Desc for backbuffer
			DXGI_MODE_DESC backBufferDesc = {};
			backBufferDesc.Width  = m_Width;
			backBufferDesc.Height = m_Height;
			backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			DXGI_SAMPLE_DESC sampleDesc = {};
			sampleDesc.Count      = 1;

			// Describe and create the swap chain.
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount	= m_BackBufferCount;
			swapChainDesc.BufferDesc	= backBufferDesc;
			swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.OutputWindow	= (HWND) NEON_CAST(DX12GraphicsSurface*, _swapchainDescriptor->Surface)->m_WindowHandle;
			swapChainDesc.SampleDesc	= sampleDesc;
			swapChainDesc.Windowed		= true;

			IDXGIFactory4* dxgiFactory;
			DX12_ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

			IDXGISwapChain* tempSwapChain;
			dxgiFactory->CreateSwapChain(NEON_CAST(DX12CommandQueue*, _commandQueue)->m_CommandQueueObj, &swapChainDesc, &tempSwapChain);
		
			// Get
			m_SwapChainObj      = static_cast<IDXGISwapChain3*>(tempSwapChain);
			m_CurrentFrameIndex = m_SwapChainObj->GetCurrentBackBufferIndex();
		}

		DX12Swapchain::~DX12Swapchain()
		{

		}

		FramebufferAttachment* DX12Swapchain::GetFramebufferAttachment(const int _i) const
		{
			// Setup framebuffer desc
			FramebufferAttachmentDescriptor framebuffetAttachmentDesc = {};
			framebuffetAttachmentDesc.Name = "BackbufferAttachment";
			framebuffetAttachmentDesc.Type = FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT;

			// Aquire image
			ID3D12Resource* image;
			DX12_ThrowIfFailed(m_SwapChainObj->GetBuffer(_i, IID_PPV_ARGS(&image)));

			// Return attachment
			return new DX12FramebufferAttachment(&framebuffetAttachmentDesc, image);
		}

		void DX12Swapchain::Resize(const int _width, const int _height)
		{

		}

		int DX12Swapchain::AquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence)
		{
			// Aquire index
			m_CurrentFrameIndex = m_SwapChainObj->GetCurrentBackBufferIndex();
			
			// Signal fence
			auto fence = NEON_CAST(DX12Fence*, _signalFence);
			NEON_CAST(DX12CommandQueue*, _commandQueue)->m_CommandQueueObj->Signal(fence->m_FenceObj, fence->m_FenceValue);

			return m_CurrentFrameIndex;
		}

		void DX12Swapchain::Present(CommandQueue* _commandQueue, const bool _vsync) const
		{
			m_SwapChainObj->Present(_vsync, 0);
		}
	}
}