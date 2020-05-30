#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./core/window/window.h"

#include <DX12/d3dx12.h>
#include "./graphics/api/directx12/dx12_error.h"

#include "./graphics/api/directx12/resources/shader/dx12_shader.h"
#include "./graphics/api/directx12/pipeline/inputLayout/dx12_input_layout.h"

#include "./graphics/objects/framebuffer/framebuffer_layout.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#include "./graphics/objects/framebuffer/framebuffer_attachment_transition_state.h"

#include "./graphics/objects/framebuffer/framebuffer_clear_flags.h"
namespace Neon
{
	namespace Graphics
	{
		struct Vertex {
			Vertex(float x, float y, float z, float r, float g, float b, float a) : pos(x, y, z), color(r, g, b, z) {}
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};

		DX12GraphicsContext::DX12GraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor)
			: GraphicsContext(_graphicsContextDescriptor)
		{
			HRESULT hr;

			IDXGIFactory4* dxgiFactory;
			DX12_ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));


			IDXGIAdapter1* adapter;

			int adapterIndex = 0;
			bool adapterFound = false;

			while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// we dont want a software device
					continue;
				}

				// we want a device that is compatible with direct3d 12 (feature level 11 or higher)
				hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
				if (SUCCEEDED(hr))
				{
					adapterFound = true;
					break;
				}

				adapterIndex++;
			}

			if (!adapterFound)
			{
				//	LOG
			}

			// Create the device
			DX12_ThrowIfFailed(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)));


			// Setup queue descriptor
			CommandQueueDescriptor queueDesc = {};
			queueDesc.Name = "Main-CommandQueue";
			queueDesc.QueueIndex = 0;
			queueDesc.Type = CommandQueueType::NEON_COMMAND_QUEUE_TYPE_DIRECT;

			// Create the queue itself
			m_CommandQueue = CommandQueue::Create(&queueDesc);

			// Setup pool descriptor
			CommandPoolDescriptor poolDesc = {};
			poolDesc.Name = "Main-CommandPool";
			poolDesc.Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;

			// Create command pool
			m_CommandPool = CommandPool::Create(&poolDesc);

			// Setup command buffer
			for (int i = 0; i < frameBufferCount; ++i)
			{
				// Setup commad buffer descriptor
				CommandBufferDescriptor commandBufferDesc = {};
				commandBufferDesc.Name = "Main-CommandBuffer: " + i;
				commandBufferDesc.Type = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
				commandBufferDesc.CommandPool = m_CommandPool;

				m_CommandBuffers[i] = CommandBuffer::Create(&commandBufferDesc);
			}

			// Use this commandBuffer for setup
			m_CommandBuffers[0]->StartRecording();

			// Setup fences
			FenceDescriptor fenceDesc = {};
			fenceDesc.Name = "AquireFence";
			m_AcuireFence = Fence::Create(&fenceDesc);

			fenceDesc.Name = "SubmitFence";
			m_SubmitFence = Fence::Create(&fenceDesc);

			GraphicsSurfaceDescriptor graphicsSurfaceDesc = {};
			graphicsSurfaceDesc.Name = "Main-GraphicsSurface";
			graphicsSurfaceDesc.Window = _graphicsContextDescriptor->Window;

			m_GraphicsSurface = GraphicsSurface::Create(&graphicsSurfaceDesc);

			// Setup the swapchain desc
			SwapchainDescriptor swapchainDesc = {};
			swapchainDesc.Name			  = "Main-Swapchain";
			swapchainDesc.Width			  = _graphicsContextDescriptor->Window->GetWindowWidth();
			swapchainDesc.Height		  = _graphicsContextDescriptor->Window->GetWindowHeight();
			swapchainDesc.BackBufferCount = 3;
			swapchainDesc.Surface		  = m_GraphicsSurface;
		
			m_Swapchain = Swapchain::Create(m_CommandQueue, &swapchainDesc);

			// Setup ShaderDescriptor
			ShaderDescriptor shaderDesc				= {};
			shaderDesc.VertexShaderPath				= "./assets/shaders/vertex_shader.hlsl";
			shaderDesc.VertexShaderFunctionName		= "main";
			shaderDesc.FragmentShaderPath			= "./assets/shaders/fragment_shader.hlsl";
			shaderDesc.FragmentShaderFunctionName	= "main";
			shaderDesc.HotReload					= false;

			// Create shader object
			ShaderReflection reflection;
			Shader* shader = Shader::Create(reflection, &shaderDesc);

			RasterizerStateDescriptor rasterizerStateDesc = {};

			// Setup GraphicsPipeline Descriptor
			GraphicsPipelineDescriptor pipelineDesc = {};
			pipelineDesc.Name		  = "Main-GraphicsPipeline";
			pipelineDesc.ImageWidth   = _graphicsContextDescriptor->Window->GetWindowWidth();
			pipelineDesc.ImageHeight  = _graphicsContextDescriptor->Window->GetWindowHeight();
			pipelineDesc.InputLayout  = reflection.Layout;
			pipelineDesc.Shader		  = shader;
			pipelineDesc.Topology	  = Topology::NEON_TOPOLOGY_TRIANGLE_LIST;
			pipelineDesc.RasterizerStateDescriptor = &rasterizerStateDesc;
			// Create the Graphics Pipeline
			m_GraphicsPipeline = GraphicsPipeline::Create(&pipelineDesc);


			// Framebuffer
			FramebufferLayout framebufferLayout = { FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT, };

			// Setup depthBufferDescriptor
			FramebufferAttachmentDescriptor framebufferAttachmentDesc = {};
			framebufferAttachmentDesc.Name = "Main-DepthBuffer";
			framebufferAttachmentDesc.Type = FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_DEPTH_STENCIL;
			framebufferAttachmentDesc.Width = 1280;
			framebufferAttachmentDesc.Height = 720;

			// Create Depth attachment
			m_DepthAttachment = FramebufferAttachment::Create(&framebufferAttachmentDesc);

			// Setup FramebufferDescriptor
			FramebufferDescriptor framebufferDesc = {};
			framebufferDesc.Name			= "Main-Framebuffer";
			framebufferDesc.Width			= 1280;
			framebufferDesc.Height			= 720;
			framebufferDesc.AttachmentCount = 1;
			framebufferDesc.DepthAttachment = true;
			framebufferDesc.Layout			= framebufferLayout;

			// Add swapchain images
			for (int i = 0; i < m_Swapchain->GetBackBufferCount(); i++)
			{
				// Create Framebuffer
				FramebufferAttachment* attachments[] = { m_Swapchain->GetFramebufferAttachment(i), m_DepthAttachment };
				framebufferDesc.Attachments = attachments;

				m_Framebuffer[i] = Framebuffer::Create(&framebufferDesc, m_GraphicsPipeline);
			}

			// Create vertex buffer

			// a triangle
			Vertex vList[] = {
				// first quad (closer to camera, blue)
				{ -0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
				{  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
				{ -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },
				{  0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f },

				// second quad (further from camera, green)
				{ -0.75f,  0.75f, 0.7f, 0.0f, 1.0f, 0.0f, 1.0f },
				{   0.0f,  0.0f,  0.7f, 0.0f, 1.0f, 0.0f, 1.0f },
				{ -0.75f,  0.0f,  0.7f, 0.0f, 1.0f, 0.0f, 1.0f },
				{   0.0f,  0.75f, 0.7f, 0.0f, 1.0f, 0.0f, 1.0f }
			};

			// Setup vertexBufferDescriptor
			VertexBufferDescriptor vertBufferDesc = {};
			vertBufferDesc.Name			= "VertexBuffer";
			vertBufferDesc.Usage		= BufferUsage::NEON_BUFFER_USAGE_STATIC;
			vertBufferDesc.Vertices		= vList;
			vertBufferDesc.Size			= sizeof(vList);
			vertBufferDesc.VertexStride	= sizeof(Vertex);
			vertBufferDesc.Offset		= 0;
			vertBufferDesc.VertexCount	= 8;
			vertBufferDesc.VMemoryPool	= nullptr;

			// Create the vertexBuffer
			m_VertexBuffer = VertexBuffer::Create(m_CommandBuffers[0], &vertBufferDesc);

			DWORD iList[] = {
				0, 1, 2, // first triangle
				0, 3, 1, // second triangle
			};

			IndexBufferDescriptor indexBufferDesc = {};
			indexBufferDesc.Name		= "IndexBuffer";
			indexBufferDesc.IndexCount	= 6;
			indexBufferDesc.Size		= sizeof(iList);
			indexBufferDesc.Format		= BufferFormat::NEON_BUFFER_FORMAT_UINT32;
			indexBufferDesc.Indices		= iList;
			indexBufferDesc.Usage		= BufferUsage::NEON_BUFFER_USAGE_STATIC;
			indexBufferDesc.IMemoryPool = nullptr;

			m_IndexBuffer = IndexBuffer::Create(m_CommandBuffers[0], &indexBufferDesc);

			// Set viewport and scissor
			m_Viewport = Viewport::Create(0, 0, _graphicsContextDescriptor->Window->GetWindowWidth(), _graphicsContextDescriptor->Window->GetWindowHeight());
			m_Scissor  = Scissor::Create(0, 0, _graphicsContextDescriptor->Window->GetWindowWidth(), _graphicsContextDescriptor->Window->GetWindowHeight());

			// Execute commandBuffer 
			m_CommandBuffers[0]->EndRecording();
			m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[0], m_AcuireFence);
		}

		DX12GraphicsContext::~DX12GraphicsContext()
		{

		}

		bool DX12GraphicsContext::Initialize()
		{
			return false;
		}

		bool DX12GraphicsContext::Terminate()
		{
			

			return true;
		}

		void DX12GraphicsContext::Present()
		{
			// Ask swapchain for new image and signal fence
			frameIndex = m_Swapchain->AquireNewImage(m_CommandQueue, m_AcuireFence);
			m_AcuireFence->WaitForFence();
			m_AcuireFence->Reset();

			// Reset Commandpool and start recording current commandBuffer
			m_CommandPool->Reset();
			m_CommandBuffers[frameIndex]->StartRecording();


			// Set pipeline state
			m_CommandBuffers[frameIndex]->SetGraphicsPipeline(m_GraphicsPipeline);

			// Transition the backbuffer
			m_CommandBuffers[frameIndex]->TransitionFramebufferAttachment(m_Framebuffer[frameIndex]->GetAttachment(0), NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT, NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER);

			// Clear
			const float clearColor[] = { 0.8f, 0.2f, 0.4f, 1.0f };
			m_CommandBuffers[frameIndex]->ClearFrameBuffer(m_Framebuffer[frameIndex], clearColor, 0, 0, NEON_CLEAR_COLOR_BIT | NEON_CLEAR_DEPTH_STENCIL_BIT);

			m_CommandBuffers[frameIndex]->BeginRenderpass(m_Framebuffer[frameIndex]);

			// draw triangle
			m_CommandBuffers[frameIndex]->SetViewport(m_Viewport);
			m_CommandBuffers[frameIndex]->SetScissor(m_Scissor);

			m_CommandBuffers[frameIndex]->SetVertexBuffer(m_VertexBuffer);
			m_CommandBuffers[frameIndex]->SetIndexBuffer(m_IndexBuffer);

			m_CommandBuffers[frameIndex]->DrawIndexed(6, 0, 0);
			m_CommandBuffers[frameIndex]->DrawIndexed(6, 0, 4);

			// Transition state back
			m_CommandBuffers[frameIndex]->TransitionFramebufferAttachment(m_Framebuffer[frameIndex]->GetAttachment(0), NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER, NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT);

			m_CommandBuffers[frameIndex]->EndRenderpass();

			m_CommandBuffers[frameIndex]->EndRecording();

			// Execure commandBuffer
			m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[frameIndex], m_SubmitFence);

			m_SubmitFence->WaitForFence();
			m_SubmitFence->Reset();

			// present the current backbuffer
			m_Swapchain->Present(m_CommandQueue, false);
		}
	}
}