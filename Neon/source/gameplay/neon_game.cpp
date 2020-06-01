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
		{
			// Setup CommandQueueLayout
			CommandQueueLayout queueLayout = { {NEON_COMMAND_QUEUE_TYPE_DIRECT, 1, true} };

			// Setup GraphicsContextDesc
			GraphicsContextDescriptor graphicsContextDesc = {};
			graphicsContextDesc.Window = _window;
			graphicsContextDesc.GraphicsApi = GraphicsAPI::DIRECTX11;
			graphicsContextDesc.QueueLayout = queueLayout;

			// Create GraphicsContext
			m_GraphicsContext = Graphics::GraphicsContext::Create(&graphicsContextDesc);
			m_GraphicsContext->Initialize();
		}
		// ==================================================================
		// Commands
		// ==================================================================
		{
			// Setup CommandQueueDesc
			CommandQueueDescriptor queueDesc = {};
			queueDesc.Name = "Main-CommandQueue";
			queueDesc.QueueIndex = 0;
			queueDesc.Type = CommandQueueType::NEON_COMMAND_QUEUE_TYPE_DIRECT;

			// Create CommandQueue
			m_CommandQueue = CommandQueue::Create(&queueDesc);
		}

		{
			// Setup CommandPoolDesc
			CommandPoolDescriptor poolDesc = {};
			poolDesc.Name = "Main-CommandPool";
			poolDesc.Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;

			// Create CommandPool
			m_CommandPool = CommandPool::Create(&poolDesc);

			// Setup CommandBuffers
			for (int i = 0; i < c_BufferCount; ++i)
			{
				// Setup CommandBufferDesc
				CommandBufferDescriptor commandBufferDesc = {};
				commandBufferDesc.Name			= "Main-CommandBuffer: " + i;
				commandBufferDesc.Type			= CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
				commandBufferDesc.CommandPool	= m_CommandPool;

				// Create CommandBuffer
				m_CommandBuffers[i] = CommandBuffer::Create(&commandBufferDesc);
			}
		}
		// ==================================================================
		// Swapchain
		// ==================================================================
		{
			// Setup GraphicsSurfaceDesc
			GraphicsSurfaceDescriptor graphicsSurfaceDesc = {};
			graphicsSurfaceDesc.Name	= "Main-GraphicsSurface";
			graphicsSurfaceDesc.Window	= _window;

			// Create GraphicsSurface
			m_GraphicsSurface = GraphicsSurface::Create(&graphicsSurfaceDesc);

			// Setup SwapchainDesc
			SwapchainDescriptor swapchainDesc = {};
			swapchainDesc.Name				= "Main-Swapchain";
			swapchainDesc.Width				= _window->GetWindowWidth();
			swapchainDesc.Height			= _window->GetWindowHeight();
			swapchainDesc.BackBufferCount	= c_BufferCount;
			swapchainDesc.Surface			= m_GraphicsSurface;

			// Create Swapchain
			m_Swapchain = Swapchain::Create(m_CommandQueue, &swapchainDesc);
		}
		// ==================================================================
		// Fence
		// ==================================================================
		{
			// Setup FenceDesc
			FenceDescriptor AfenceDesc = { };
			AfenceDesc.Name = "acquireFence";

			// Create Fence
			m_AcquireFence = Fence::Create(&AfenceDesc);
		}
		{
			// Setup FenceDesc
			FenceDescriptor SfenceDesc = { };
			SfenceDesc.Name = "submitFence";

			// Create Fence
			m_SubmitFence = Fence::Create(&SfenceDesc);
		}
		// ==================================================================
		// Pipeline
		// ==================================================================
		{
			// Setup ShaderDesc
			ShaderDescriptor shaderDesc = {};
			//shaderDesc.VertexShaderPath = "./assets/shaders/vert.spv";
			shaderDesc.VertexShaderPath = "./assets/shaders/vertex_shader.hlsl";
			shaderDesc.VertexShaderFunctionName = "main";
			//shaderDesc.FragmentShaderPath = "./assets/shaders/frag.spv";
			shaderDesc.FragmentShaderPath = "./assets/shaders/fragment_shader.hlsl";
			shaderDesc.FragmentShaderFunctionName = "main";
			shaderDesc.HotReload = false;

			// Create Shader
			
			ShaderReflection reflection;
			m_Shader = Shader::Create(reflection, &shaderDesc);
	
			RasterizerStateDescriptor rasterizerStateDesc = {};
	
			// Setup GraphicsPipelineDesc
			GraphicsPipelineDescriptor pipelineDesc = {};
			pipelineDesc.Name						= "Main-GraphicsPipeline";
			pipelineDesc.ImageWidth					= 1280;
			pipelineDesc.ImageHeight				= 720;
			pipelineDesc.Shader						= m_Shader;
			pipelineDesc.RasterizerStateDescriptor	= &rasterizerStateDesc;
			pipelineDesc.InputLayout				= reflection.Layout;
			pipelineDesc.Topology					= Topology::NEON_TOPOLOGY_TRIANGLE_LIST;
	
			// Create GraphicsPipeline
			m_Pipeline = GraphicsPipeline::Create(&pipelineDesc);
		}
		// ==================================================================
		// Framebuffer
		// ==================================================================
		{
			// FramebufferLayout
			FramebufferLayout framebufferLayout = { FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT, };

			// Setup FramebufferDesc
			FramebufferDescriptor framebufferDesc = {};
			framebufferDesc.Name			= "Main-Framebuffer";
			framebufferDesc.Width			= 1280;
			framebufferDesc.Height			= 720;
			framebufferDesc.AttachmentCount = 1;
			framebufferDesc.DepthAttachment = false;
			framebufferDesc.Layout			= framebufferLayout;

			// Loop to create Framebuffers
			for (int i = 0; i < frameBufferCount; ++i)
			{
				// Get attachment
				FramebufferAttachment** att = new FramebufferAttachment*[1];
				att[0] = m_Swapchain->GetFramebufferAttachment(i);

				framebufferDesc.Attachments = att;

				// Create Framebuffer
				m_Framebuffer[i] = Framebuffer::Create(&framebufferDesc, m_Pipeline);
			}
		}
		// ==================================================================
		// VertexBuffer
		// ==================================================================
		{
			m_CommandBuffers[0]->StartRecording();
	
			float vertices[] = {
			 -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			 -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			  0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			
			 -0.75f,  0.75f, 0.7f, 0.0f, 1.0f, 0.0f, 1.0f,
			   0.0f,  0.0f,  0.7f, 0.0f, 1.0f, 0.0f, 1.0f,
			 -0.75f,  0.0f,  0.7f, 0.0f, 1.0f, 0.0f, 1.0f,
			   0.0f,  0.75f, 0.7f, 0.0f, 1.0f, 0.0f, 1.0f,
			};
	
			// Setup MemoryPoolDesc
			MemoryPoolDescriptor memoryPoolDesc = {};
			memoryPoolDesc.Name		= "VertexMemoryPool";
			memoryPoolDesc.Access	= MemoryAccess::NEON_MEMORY_ACCESS_GPU_ONLY;
			memoryPoolDesc.Size		= sizeof(vertices);
	
			// Create MemoryPool
			m_VmemoryPool = MemoryPool::Create(&memoryPoolDesc);
	
			// Setup VertexBufferDesc
			VertexBufferDescriptor vertexBufferDesc = {};
			vertexBufferDesc.Name			= "VertexBuffer";
			vertexBufferDesc.Size			= sizeof(vertices);
			vertexBufferDesc.Usage			= BufferUsage::NEON_BUFFER_USAGE_STATIC;
			vertexBufferDesc.Vertices		= vertices;
			vertexBufferDesc.VMemoryPool	= m_VmemoryPool;
			vertexBufferDesc.VertexCount	= 8;
			vertexBufferDesc.VertexStride	= sizeof(float) * 7;
			vertexBufferDesc.Offset			= 0;
	
			// Create VertexBuffer
			m_VertexBuffer = VertexBuffer::Create(m_CommandBuffers[0], &vertexBufferDesc);
		}
		// ==================================================================
		// IndexBuffer
		// ==================================================================
		{
			uint16_t indices[] = { 0, 1, 2, 3, 1, 0,
								4, 5, 6, 7, 5, 4 };
	
			// Setup MemoryPoolDesc
			MemoryPoolDescriptor imemoryPoolDesc = {};
			imemoryPoolDesc.Name	= "VertexMemoryPool";
			imemoryPoolDesc.Access	= MemoryAccess::NEON_MEMORY_ACCESS_GPU_ONLY;
			imemoryPoolDesc.Size	= sizeof(indices);
	
			// Create MemoryPool
			m_ImemoryPool = MemoryPool::Create(&imemoryPoolDesc);
	
			// Setup IndexBufferDesc
			IndexBufferDescriptor indexBufferDesc = {};
			indexBufferDesc.Name		= "IndexBuffer";
			indexBufferDesc.Size		= sizeof(indices);
			indexBufferDesc.Usage		= BufferUsage::NEON_BUFFER_USAGE_STATIC;
			indexBufferDesc.Indices		= indices;
			indexBufferDesc.IMemoryPool = m_ImemoryPool;
			indexBufferDesc.IndexCount	= 12;
			indexBufferDesc.Format = BufferFormat::NEON_BUFFER_FORMAT_UINT32;
	
			// Create IndexBuffer
			m_IndexBuffer = IndexBuffer::Create(m_CommandBuffers[0], &indexBufferDesc);
	
			m_CommandBuffers[0]->EndRecording();
			m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[0], m_SubmitFence);
	
			m_SubmitFence->WaitForFence();
			m_SubmitFence->Reset();
		}
		// ==================================================================
		// CommandObjects
		// ==================================================================
		{
			// Set viewport and scissor
			m_Viewport = Viewport::Create(0.0f, 0.0f, _window->GetWindowWidth(), _window->GetWindowHeight());
			m_Scissor   = Scissor::Create(0.0f, 0.0f, _window->GetWindowWidth(), _window->GetWindowHeight());

		}

		return true;
	}

	bool NeonGame::Terminate()
	{
		// Destroy Swapchain
		delete m_Swapchain;
		delete m_GraphicsSurface;

		// Destroy Command
		for (int i = 0; i < c_BufferCount; ++i)
			delete m_CommandBuffers[i];

		delete m_CommandPool;
		delete m_CommandQueue;

		// Destroy GraphicsContext
		m_GraphicsContext->Terminate();
		delete m_GraphicsContext;

		return true;
	}

	void NeonGame::Update(const float _dt)
	{

	}

	void NeonGame::Render()
	{
		// Get new frameIndex
		uint32_t frameIndex = m_Swapchain->AcquireNewImage(m_CommandQueue, m_AcquireFence);
		
		// Start recording commands
		m_CommandPool->Reset();
		m_CommandBuffers[frameIndex]->StartRecording();
	
		m_CommandBuffers[frameIndex]->SetGraphicsPipeline(m_Pipeline);
	
	
		m_CommandBuffers[frameIndex]->TransitionFramebufferAttachment(m_Framebuffer[frameIndex]->GetAttachment(0), NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT, NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER);
	
		const float clearColor[] = { 0.8f, 0.2f, 0.4f, 1.0f };
		m_CommandBuffers[frameIndex]->ClearFrameBuffer(m_Framebuffer[frameIndex], clearColor, 0, 0, NEON_CLEAR_COLOR_BIT);// | NEON_CLEAR_DEPTH_STENCIL_BIT);
	
	
		m_CommandBuffers[frameIndex]->BeginRenderpass(m_Framebuffer[frameIndex]);
	
		m_CommandBuffers[frameIndex]->SetViewport(m_Viewport);
		m_CommandBuffers[frameIndex]->SetScissor(m_Scissor);
	
		m_CommandBuffers[frameIndex]->SetVertexBuffer(m_VertexBuffer);
		m_CommandBuffers[frameIndex]->SetIndexBuffer(m_IndexBuffer);
	
		m_CommandBuffers[frameIndex]->DrawIndexed(m_IndexBuffer->GetIndexCount(), 0, 0);
	
		m_CommandBuffers[frameIndex]->EndRenderpass();
	
		m_CommandBuffers[frameIndex]->TransitionFramebufferAttachment(m_Framebuffer[frameIndex]->GetAttachment(0), NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER, NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT);
	
		m_CommandBuffers[frameIndex]->EndRecording();
	
		// Wait for fence
		m_AcquireFence->WaitForFence();
		m_AcquireFence->Reset();
	
		m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[frameIndex], m_SubmitFence);
	
		m_SubmitFence->WaitForFence();
		m_SubmitFence->Reset();

		m_Swapchain->Present(m_CommandQueue, false);
	}
}