#pragma once
#include "./core/game.h"

// Graphics includes
#include "./graphics/graphics_context.h"

#include "./graphics/objects/command/command_queue.h"
#include "./graphics/objects/command/command_buffer.h"

#include "./graphics/objects/swapchain/swapchain.h"

using namespace Neon::Graphics;
namespace Neon
{
	// Define buffer count
	const int c_BufferCount = 3;

	class NeonGame : public Core::Game
	{
	public:
		NeonGame() = default;
		NeonGame(const std::string& _gameName, const VersionID _versionID);

		virtual bool Initialize(Core::Window* _window) override;
		virtual bool Terminate() override;

		virtual void Update(const float _dt) override;
		virtual void Render() override;

	private:

		GraphicsContext*  m_GraphicsContext;

	//	CommandQueue*     m_CommandQueue;
	//
	//	CommandPool*      m_CommandPool;
	//	CommandBuffer*    m_CommandBuffers[c_BufferCount];

	//	Swapchain*		  m_Swapchain;
	//	GraphicsSurface*  m_GraphicsSurface;
	};
}