#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./core/window/window.h"

#include <DX12/d3dx12.h>
#include "./graphics/api/directx12/dx12_error.h"

#include <dxgi1_4.h>

namespace Neon
{
	namespace Graphics
	{
		DX12GraphicsContext::DX12GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor)
			: GraphicsContext(_graphicsContextDescriptor)
		{
#if defined(_DEBUG)
			// Always enable the debug layer before doing anything DX12 related
			// so all possible errors generated while creating DX12 objects
			// are caught by the debug layer.
			Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
			DX12_ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
			debugInterface->EnableDebugLayer();
#endif

			HRESULT hr;

			IDXGIFactory4* dxgiFactory;
			UINT createFactoryFlags = 0;
#if defined(_DEBUG)
			createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
			DX12_ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));


			IDXGIAdapter1* adapter;

			int adapterIndex = 0;
			bool adapterFound = false;

			while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// we dont want a software device
					continue;
				}

				// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
				hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
				if (SUCCEEDED(hr))
				{
					adapterFound = true;
					break;
				}
				adapter->Release();
				adapterIndex++;
			}

			// Create the device
			DX12_ThrowIfFailed(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));

			adapter->Release();
			dxgiFactory->Release();
		}

		DX12GraphicsContext::~DX12GraphicsContext()
		{
			m_Device->Release();
		}

		bool DX12GraphicsContext::Initialize()
		{
			return false;
		}

		bool DX12GraphicsContext::Terminate()
		{
			

			return true;
		}
	}
}