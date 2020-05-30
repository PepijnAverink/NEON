#include "./graphics/graphics_context.h"
#include "./core/window/window.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)
#include "./graphics/api/directx11/dx11_graphics_context.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)
#include "./graphics/api/directx12/dx12_graphics_context.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/vk_graphics_context.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		static GraphicsContext* s_GraphicsContext = nullptr;

		GraphicsContext* GraphicsContext::Create(const GraphicsContextDescriptor* _graphicsContextDescriptor)
		{
			// Setup api
			GraphicsAPI api = _graphicsContextDescriptor->GraphicsApi;
			GraphicsDriver::Create(api);

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKGraphicsContext(_graphicsContextDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11GraphicsContext(_graphicsContextDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12GraphicsContext(_graphicsContextDescriptor);

			printf("[ERROR] GraphicsContext::Create() No valid api abstraction for found for object: GraphicsContext");
			return nullptr;
		}

		GraphicsContext* GraphicsContext::GetGraphicsContext()
		{
			NEON_CHECK(s_GraphicsContext == nullptr, "[WARNING] GraphicsContext::GetGraphicsContext() - No valid instance of GraphicsContxt was found.");
			return s_GraphicsContext;
		}

		GraphicsContext::~GraphicsContext()
		{
			// Destroy api
			GraphicsDriver::Destroy();
		}

		GraphicsContext::GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor)
			: m_ClientWidth(_graphicsContextDescriptor->Window->GetWindowWidth())
			, m_ClientHeight(_graphicsContextDescriptor->Window->GetWindowHeight())
			, m_ScreenWidth(_graphicsContextDescriptor->Window->GetScreenWidth())
			, m_ScreenHeight(_graphicsContextDescriptor->Window->GetScreenHeight())
			, m_VSync(_graphicsContextDescriptor->Window->GetVSync())
		{ 
			s_GraphicsContext = this;
		}
	}
}