#include "./gameplay/neon_game.h"
#include "./core/window/window.h"

// TEmp
#include "./graphics/graphics_driver.h"

namespace Neon
{
	NeonGame::NeonGame(const std::string& _gameName, const VersionID _versionID)
		: Game(_gameName, _versionID)
	{ }

	bool NeonGame::Initialize(Core::Window* _window)
	{
		// ==================================================================
		// GraphicsContext
		// ==================================================================

		// Setup GraphicsContextDesc
		GraphicsContextDescriptor graphicsContextDesc = {};
		graphicsContextDesc.Window		= _window;
		graphicsContextDesc.GraphicsApi = GraphicsAPI::VULKAN;

		// Create graphics context
		m_GraphicsContext = Graphics::GraphicsContext::Create(&graphicsContextDesc);
		m_GraphicsContext->Initialize();


		return true;
	}

	bool NeonGame::Terminate()
	{
		// Destroys graphics context
		m_GraphicsContext->Terminate();
		delete m_GraphicsContext;

		return true;
	}

	void NeonGame::Update(const float _dt)
	{

	}

	void NeonGame::Render()
	{
		m_GraphicsContext->Present();
	}
}