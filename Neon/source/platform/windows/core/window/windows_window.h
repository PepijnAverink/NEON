#pragma once
#include "./core/window/window.h"

#include <windows.h>

namespace Neon
{
	namespace Core
	{
		class WindowsWindow final : public Window
		{
		public:
			WindowsWindow(WindowDescriptor& _windowDescriptor);

			virtual bool Intialize() override;
			virtual bool Terminate() override;

			virtual void Update() override;

			virtual bool Show() const override;
			virtual bool Hide() const override;

			// Getters
			virtual inline void* GetNativeWindowHandle() const override { return m_WindowHandle; };

		private:
			int GetDefaultBorderHeight();
			int GetDefaultBorderWidth();

			HWND m_WindowHandle;
		};
	}
}