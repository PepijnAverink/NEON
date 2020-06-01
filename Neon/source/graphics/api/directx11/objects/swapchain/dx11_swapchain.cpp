#include "./graphics/api/directx11/objects/swapchain/dx11_swapchain.h"
#include "./graphics/api/directx11/objects/swapchain/dx11_graphics_surface.h"
#include "./graphics/api/directx11/objects/framebuffer/dx11_framebuffer_attachment.h"
#include "./graphics/api/directx11/objects/sync/dx11_fence.h"
#include "./graphics/api/directx11/objects/command/dx11_command_queue.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./utilities/casting/casting_helper.h"

#include <dxgi.h>

namespace Neon
{
	namespace Graphics
	{
		DX11Swapchain::DX11Swapchain(CommandQueue* _commandQueue, const SwapchainDescriptor* _swapchainDescriptor)
			: Swapchain(_swapchainDescriptor)
		{
			// Initialize the swap chain description
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			
			// Build the description object
			swapChainDesc.BufferCount							= m_BackBufferCount;
			swapChainDesc.BufferDesc.Width						= _swapchainDescriptor->Width;
			swapChainDesc.BufferDesc.Height						= _swapchainDescriptor->Height;
			swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferDesc.RefreshRate.Numerator		= _swapchainDescriptor->Adapter->DisplayModes[0].Numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator	= _swapchainDescriptor->Adapter->DisplayModes[0].Denominator;
			swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow							= (HWND)NEON_CAST(DX11GraphicsSurface*, _swapchainDescriptor->Surface)->m_WindowHandle;
			swapChainDesc.SampleDesc.Count						= 1;
			swapChainDesc.SampleDesc.Quality					= 0;
			swapChainDesc.Windowed								= true;		// CHANGE FULL screen
			swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
			swapChainDesc.Flags									= 0;

			// Create swapchain
			IDXGISwapChain* tempSwapChain;
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetFactory()->CreateSwapChain(DX11GraphicsContext::GetInstance()->GetGraphicsDevice(), &swapChainDesc, &tempSwapChain));
	
			// Get
			m_SwapchainObj		= static_cast<IDXGISwapChain3*>(tempSwapChain);
			m_CurrentFrameIndex = m_SwapchainObj->GetCurrentBackBufferIndex();

			// cleanup
			tempSwapChain->Release();
		}

		DX11Swapchain::~DX11Swapchain()
		{

		}

		FramebufferAttachment* DX11Swapchain::GetFramebufferAttachment(const int _i) const
		{
			// Setup framebuffer desc
			FramebufferAttachmentDescriptor framebuffetAttachmentDesc = {};
			framebuffetAttachmentDesc.Name = "BackbufferAttachment";
			framebuffetAttachmentDesc.Type = FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT;

			// Aquire image
			ID3D11Texture2D* image;
			DX11_ThrowIfFailed(m_SwapchainObj->GetBuffer(_i, IID_PPV_ARGS(&image)));

			// Return attachment
			return new DX11FramebufferAttachment(&framebuffetAttachmentDesc, image);
		}

		void DX11Swapchain::Resize(const int _width, const int _height)
		{

		}

		unsigned int DX11Swapchain::AcquireNewImage(CommandQueue* _commandQueue, Fence* _signalFence)
		{
			m_CurrentFrameIndex = m_SwapchainObj->GetCurrentBackBufferIndex();
			
			// Signal fence
			auto fence = NEON_CAST(DX11Fence*, _signalFence);
		//	NEON_CAST(DX11CommandQueue*, _commandQueue)->m_CommandQueueObj->Signal(fence->m_FenceObj, fence->m_FenceValue);

			return m_CurrentFrameIndex;
		}

		void DX11Swapchain::Present(CommandQueue* _commandQueue, const bool _vsync) const
		{
			m_SwapchainObj->Present(_vsync, 0);
		}
	}
}