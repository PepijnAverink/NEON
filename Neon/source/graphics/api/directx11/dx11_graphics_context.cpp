#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./graphics/hardware/adapter/vendor_helper.h"

#include "./utilities/string_utils.h"

namespace Neon
{
	namespace Graphics
	{
		DX11GraphicsContext::DX11GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor)
			: GraphicsContext(_graphicsContextDescriptor)
		{ }

		DX11GraphicsContext::~DX11GraphicsContext()
		{

		}

		bool DX11GraphicsContext::Initialize()
		{
			// Create factory
			DX11_ThrowIfFailed(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_Factory));

			GetAdapters();
			CreateDevice();

			// Setup commad buffer descriptor
			CommandBufferDescriptor commandBufferDesc = {};
			commandBufferDesc.Name = "Main-CommandBuffer:";
			commandBufferDesc.Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
			commandBufferDesc.CommandPool = nullptr;

			m_CommandBuffer = CommandBuffer::Create(&commandBufferDesc);

			return false;
		}

		bool DX11GraphicsContext::Terminate()
		{
			return false;
		}

		void DX11GraphicsContext::GetAdapters()
		{
			IDXGIAdapter* adapter;
			for (UINT i = 0; m_Factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
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
		}

		void DX11GraphicsContext::CreateDevice()
		{
			// Get the adapter
			IDXGIAdapter* adapter;
			m_Factory->EnumAdapters(m_CurrentAdapter, &adapter);
			
			unsigned int flags = 0;
			// IF DEBUG
			flags = D3D11_CREATE_DEVICE_DEBUG;

			// Create old device
			ID3D11Device*        temp_device;
			ID3D11DeviceContext* temp_device_context;
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
			DX11_ThrowIfFailed(D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1, D3D11_SDK_VERSION, &temp_device, NULL, &temp_device_context));
			
			// Create new device
			temp_device->QueryInterface(__uuidof(ID3D11Device5), (void**)&m_Device);
			m_DeviceContext = static_cast<ID3D11DeviceContext4*>(temp_device_context);

			// Cleanup
			temp_device->Release();
			adapter->Release();
		}
	}
}