#include "./core/engine.h"
#include "./core/core_defines.h"
#include "./core/window/window.h"

// Graphics
#include "./graphics/graphics_context.h"
#include "./graphics/graphics_driver.h"

namespace Neon
{
	namespace Core
	{
		// Singleton pointer to Engine class
		static Engine* s_Engine = nullptr;

		void Engine::Create()
		{
			// Check if an instance already exists
			if (s_Engine != nullptr) 
			{
				printf("[ERROR] Engine::Create() - User called Create() with a valid Engine* already active.\n\t Not overwritten...");
				return;
			}

			// Creates an engine instance
			s_Engine = new Engine();
			s_Engine->Initialize();
		}

		void Engine::Destroy()
		{
			// Check if an instance exists
			if (s_Engine == nullptr)
			{
				printf("[WARNING] Engine::Destroy() - User called Destroy() without a valid Engine* being active.");
				return;
			}

			// Destroys the engine instance
			s_Engine->Terminate();
			delete s_Engine;
		}

		Engine* Engine::GetInstance()
		{
			NEON_CHECK(s_Engine == nullptr, "[WARNING] Engine::GetInstance() - Engine* was not initialized properly.");
			return s_Engine;
		}

		Engine::Engine()
			: m_EngineName("")
			, m_EngineVersion(0)
			, m_Window(nullptr)
			, m_GraphicsContext(nullptr)
			, m_Game(nullptr)
			, m_Running(false)
		{ }

		Engine::Engine(const std::string& _engineName, const VersionID _versionID)
			: m_EngineName(_engineName)
			, m_EngineVersion(_versionID)
			, m_Window(nullptr)
			, m_GraphicsContext(nullptr)
			, m_Game(nullptr)
			, m_Running(false)
		{ }

		bool Engine::Initialize()
		{
			// Setups the window properties
			WindowDescriptor windowDesc;
			windowDesc.Title  = "Neon Game";
			windowDesc.Width  = 1280;
			windowDesc.Height = 720;
			windowDesc.VSync  = false;

			// Create the actual window
			m_Window = Window::Create(windowDesc);
			m_Window->Intialize();

			// Create graphics context
			Graphics::GraphicsDriver::Create(Graphics::GraphicsAPI::VULKAN);
			m_GraphicsContext = Graphics::GraphicsContext::Create(m_Window);
			m_GraphicsContext->Initialize();

			return true;
		}

		bool Engine::Terminate()
		{
			// Destroys game
			if (m_Game != nullptr)
			{
				m_Game->Terminate();
				delete m_Game; m_Game = nullptr;
			}

			// Destroys graphics context
			m_GraphicsContext->Terminate();
			delete m_GraphicsContext; m_GraphicsContext = nullptr;

			Graphics::GraphicsDriver::Destroy();

			// Destroys window
			m_Window->Terminate();
			delete m_Window; m_Window = nullptr;

			return true;
		}

		// TODO:: Define error codes
		int Engine::RunGame(Game* _game)
		{
			NEON_CHECK(m_Game != nullptr, "[WARNING] Engine::RunGame() - A valid instance of game already, this will be overwritten.");
			if (_game == nullptr)
			{
				printf("[ERROR] Engine::RunGame() - A non-valid Game* was given by the user.");
				return -1;
			}

			// Initialize game
			m_Game = _game;
			m_Game->Initialize();

			m_Window->Show();

			// Start the game loop
			m_Running = true;
			while (m_Running)
			{
				// Update window
				m_Window->Update();


				m_Game->Update(1.0f);
				m_Game->Render();

				m_GraphicsContext->Present();
			}

			m_Window->Hide();

			return 0;
		}

		void Engine::RequestQuit()
		{
			printf("[info] Engine::RequestQuit() - Quit has been requested.");
			m_Running = false;
		}
	}
}