#include "./platform/windows/core/window/windows_window.h"
#include "./core/engine.h"

#include "../resource.h"

namespace Neon
{
	namespace Core
	{
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		WindowsWindow::WindowsWindow(WindowDescriptor& _windowDescriptor)
			: Window(_windowDescriptor)
		{ }

		bool WindowsWindow::Intialize()
		{
			WNDCLASSEX wc;
			ZeroMemory(&wc, sizeof(WNDCLASSEX));

			// fill in the struct with the needed information
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_HREDRAW | CS_VREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = "WindowClass";
			wc.hIconSm = LoadIcon(wc.hInstance, IDI_APPLICATION);

			// register the window class
			RegisterClassEx(&wc);

			// create the window and use the result as the handle
			m_WindowHandle = CreateWindowEx(NULL, "WindowClass", m_WindowTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
				m_Width + GetDefaultBorderWidth(), m_Height + GetDefaultBorderHeight(), NULL, NULL, GetModuleHandle(NULL), NULL);

			return true;
		}

		bool WindowsWindow::Terminate()
		{
			return true;
		}

		void WindowsWindow::Update()
		{
			// Focussed
			m_Focussed = (m_WindowHandle == GetForegroundWindow());

			// Handle messages
			MSG msg;
			while (PeekMessage(&msg, m_WindowHandle, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		bool WindowsWindow::Show() const
		{
			ShowWindow(m_WindowHandle, SW_SHOW);
			return true;
		}

		bool WindowsWindow::Hide() const
		{
			ShowWindow(m_WindowHandle, SW_HIDE);
			return true;
		}

		int WindowsWindow::GetDefaultBorderHeight()
		{
			const int sizingBorder = GetSystemMetrics(SM_CYFRAME);
			const int captionHeight = GetSystemMetrics(SM_CYCAPTION);
			const int borderPadding = GetSystemMetrics(SM_CXPADDEDBORDER);
			return    sizingBorder * 2 + borderPadding * 2 + captionHeight;
		}

		int WindowsWindow::GetDefaultBorderWidth()
		{
			const int sizingBorder = GetSystemMetrics(SM_CYFRAME);
			const int borderPadding = GetSystemMetrics(SM_CXPADDEDBORDER);
			return    sizingBorder * 2 + borderPadding * 2;
		}

		static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			// Sort through the messages
			switch (message)
			{

				// close the applCcation entirely
				case WM_DESTROY:
				{
					Engine::GetInstance()->RequestQuit();
				//	PostQuitMessage(0);
					break;
				}
			}

			// Handle any messages the switch statement didn't
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
	}
}