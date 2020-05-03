#include "./graphics/api/directx12/objects/swapchain/dx12_swapchain.h"
#include "./graphics/api/directx12/objects/command/dx12_command_queue.h"
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
			backBufferDesc.Width  = _swapchainDescriptor->Width;
			backBufferDesc.Height = _swapchainDescriptor->Height;
			backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

			DXGI_SAMPLE_DESC sampleDesc = {};
			sampleDesc.Count      = 1;

			// Describe and create the swap chain.
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount	= _swapchainDescriptor->BackBufferCount;
			swapChainDesc.BufferDesc	= backBufferDesc;
			swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.OutputWindow	= (HWND)_swapchainDescriptor->Window->GetNativeWindowHandle();
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
		void DX12Swapchain::Resize(const int _width, const int _height)
		{

		}

		int DX12Swapchain::AquireNewImage(Fence* _signalFence)
		{
			return 0;
		}
	}
}