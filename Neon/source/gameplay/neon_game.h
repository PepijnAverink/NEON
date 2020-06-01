#pragma once
#include "./core/game.h"

// Graphics includes
#include "./graphics/graphics_context.h"

#include "./graphics/objects/command/command_queue.h"
#include "./graphics/objects/command/command_buffer.h"

#include "./graphics/objects/swapchain/swapchain.h"

#include "./graphics/resources/shader/shader.h"
#include "./graphics/pipeline/graphics_pipeline.h"

#include "./graphics/objects/framebuffer/framebuffer.h"

#include "./graphics/resources/buffer/vertex_buffer.h"
#include "./graphics/resources/buffer/index_buffer.h"
#include "./graphics/resources/memory/memory_pool.h"

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
		// Context
		GraphicsContext*  m_GraphicsContext;

		// Commands
		CommandQueue*     m_CommandQueue;
	
		CommandPool*      m_CommandPool;
		CommandBuffer*    m_CommandBuffers[c_BufferCount];

		// Swapchain
		Swapchain*		  m_Swapchain;
		GraphicsSurface*  m_GraphicsSurface;

		Fence*			  m_SubmitFence;
		Fence*			  m_AcquireFence;

		// Pipeline
		Shader*			  m_Shader;
		GraphicsPipeline* m_Pipeline;

		// Framebuffer
		Framebuffer*	  m_Framebuffer[c_BufferCount];

		// Buffers
		VertexBuffer*	  m_VertexBuffer;
		MemoryPool*		  m_VmemoryPool;
						  
		IndexBuffer*	  m_IndexBuffer;
		MemoryPool*		  m_ImemoryPool;

		// CommandObjects
		Viewport*		  m_Viewport;
		Scissor*		  m_Scissor;

	};
}