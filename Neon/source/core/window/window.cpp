#include "./core/window/window.h"
#include "./core/core_defines.h"

#if defined(NEON_PLATFORM_WINDOWS)
#include "./platform/windows/core/window/windows_window.h"
#endif

namespace Neon
{
	namespace Core
	{
		Window* Window::Create(WindowDescriptor& _windowDescriptor)
		{
#if defined(NEON_PLATFORM_WINDOWS)
			return new WindowsWindow(_windowDescriptor);
#endif

			printf("[ERROR] Window::Create() - No valid implementation for the window class was found on the current platform.");
			return nullptr;
		}

		Window::Window(WindowDescriptor& _windowDescriptor)
			: m_WindowTitle(_windowDescriptor.Title)
			, m_Width(_windowDescriptor.Width)
			, m_Height(_windowDescriptor.Height)
			, m_VSync(_windowDescriptor.VSync)
		{ }
	}
}