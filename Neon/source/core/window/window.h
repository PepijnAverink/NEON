#pragma once
#include "./core/window/window_descriptor.h"
namespace Neon
{
	namespace Core
	{
		class Window
		{
		public:
			static Window* Create(WindowDescriptor& _windowDescriptor);

			virtual bool Intialize() = 0;
			virtual bool Terminate() = 0;

			virtual void Update() = 0;

			// May not work on certain platforms 
			// Returns wether not operation was completed
			virtual bool Show() const = 0;
			virtual bool Hide() const = 0;

			// Getters
			inline int GetWindowWidth()  const { return m_Width; }
			inline int GetWindowHeight() const { return m_Height; }

			inline bool GetVSync() const { return m_VSync; }

			virtual inline void* GetNativeWindowHandle() const = 0;
			inline bool IsFocussed() const { return m_Focussed; }

		protected:
			Window(WindowDescriptor& _windowDescriptor);

			std::string m_WindowTitle;
			int m_Width;
			int m_Height;

			bool m_VSync;
			bool m_Focussed;
		};
	}
}