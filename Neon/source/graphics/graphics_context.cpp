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

		GraphicsContext* GraphicsContext::Create(Core::Window* _window)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKGraphicsContext(_window);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11GraphicsContext(_window);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12GraphicsContext(_window);

			printf("[ERROR] GraphicsContext::Create() No valid api abstraction for found for object: GraphicsContext");
			return nullptr;
		}

		GraphicsContext* GraphicsContext::GetGraphicsContext()
		{
			NEON_CHECK(s_GraphicsContext == nullptr, "[WARNING] GraphicsContext::GetGraphicsContext() - No valid instance of GraphicsContxt was found.");
			return s_GraphicsContext;
		}

		GraphicsContext::GraphicsContext(Core::Window* _window)
			: m_ClientWidth(_window->GetWindowWidth())
			, m_ClientHeight(_window->GetWindowHeight())
			, m_ScreenWidth(_window->GetScreenWidth())
			, m_ScreenHeight(_window->GetScreenHeight())
			, m_VSync(_window->GetVSync())
		{ 
			s_GraphicsContext = this;
		}
	}
}