#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./graphics/hardware/adapter/vendor_helper.h"

#include "./utilities/string_utils.h"

namespace Neon
{
	namespace Graphics
	{
		DX11GraphicsContext::DX11GraphicsContext(Core::Window* _window)
			: GraphicsContext(_window)
		{ }

		DX11GraphicsContext::~DX11GraphicsContext()
		{

		}

		bool DX11GraphicsContext::Initialize()
		{
			GetAdapters();
			CreateDevice();

			return false;
		}

		bool DX11GraphicsContext::Terminate()
		{
			return false;
		}

		void DX11GraphicsContext::Present()
		{

		}

		void DX11GraphicsContext::GetAdapters()
		{
			// Create the factory
			IDXGIFactory* factory;
			IDXGIAdapter* adapter;
			DX11_ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));

			for (UINT i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
			{
				// Get adapter descriptor
				DXGI_ADAPTER_DESC adapterDesc;
				DX11_ThrowIfFailed(adapter->GetDesc(&adapterDesc));

				// Store adapter info
				VideoAdapter* vadapter = new VideoAdapter();
				vadapter->Name   = WStringToString(std::wstring(adapterDesc.Description));
				vadapter->Vendor = GetVendorByPCI(adapterDesc.VendorId);
				vadapter->Type   = (vadapter->Vendor == "unknown") ? NEON_VIDEO_ADAPTER_TYPE_SOFTWARE : NEON_VIDEO_ADAPTER_TYPE_HARDWARE;

				vadapter->VideoMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024 / 1024);

				// Check adapter type
				// Set to default if its a software adapter
				if (vadapter->Type == NEON_VIDEO_ADAPTER_TYPE_SOFTWARE)
				{
					DisplayMode displayMode;
					displayMode.ResolutionWidth  = m_ScreenWidth;
					displayMode.ResolutionHeight = m_ScreenHeight;

					displayMode.Numerator = 60;
					displayMode.Denominator = 1;
					vadapter->DisplayModes.push_back(displayMode);
					continue;
				}

				// Get number of modes
				unsigned int numModes;
				IDXGIOutput* adapterOutput;
				DX11_ThrowIfFailed(adapter->EnumOutputs(0, &adapterOutput));
				DX11_ThrowIfFailed(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

				// Error checking
				if (numModes < 1)
				{
					// Cleanup
					delete vadapter;
					adapterOutput->Release();
					adapter->Release();

					continue;
				}

				// Fill the display mode list
				DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
				DX11_ThrowIfFailed(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList));

				// Get the refresh rate of the matching monitor
				for (int i = 0; i < numModes; i++)
				{
					unsigned int horizontal = displayModeList[i].Width;
					unsigned int vertical   = displayModeList[i].Height;

					if (horizontal == m_ScreenWidth && vertical == m_ScreenHeight)
					{
						DisplayMode displayMode;
						displayMode.ResolutionWidth = horizontal;
						displayMode.ResolutionHeight = vertical;

						displayMode.Numerator   = displayModeList[i].RefreshRate.Numerator;
						displayMode.Denominator = displayModeList[i].RefreshRate.Denominator;
						vadapter->DisplayModes.push_back(displayMode);
					}
				}

				// Add adapter
				m_Adapters.push_back(vadapter);

				// Cleanup
				delete[] displayModeList;
				adapterOutput->Release();
				adapter->Release();
			}

			// Release factory
			factory->Release();
		}

		void DX11GraphicsContext::CreateDevice()
		{
			// Get the adapter
			IDXGIFactory* factory;
			IDXGIAdapter* adapter;
			DX11_ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory));
			factory->EnumAdapters(m_CurrentAdapter, &adapter);
			
			unsigned int flags = 0;
			// IF DEBUG
			flags = D3D11_CREATE_DEVICE_DEBUG;

			// Create the device
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
			DX11_ThrowIfFailed(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1, D3D11_SDK_VERSION, &m_Device, NULL, &m_DeviceContext));

			// Cleanup
			factory->Release();
			adapter->Release();
		}
	}
}