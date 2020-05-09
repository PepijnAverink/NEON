#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./core/window/window.h"

#include <DX12/d3dx12.h>
#include "./graphics/api/directx12/dx12_error.h"

#include "./graphics/api/directx12/resources/shader/dx12_shader.h"
#include "./graphics/api/directx12/pipeline/inputLayout/dx12_input_layout.h"

#include "./graphics/objects/command_generic/topology.h"

#include "./graphics/objects/framebuffer/framebuffer_layout.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer_attachment.h"
#include "./graphics/objects/framebuffer/framebuffer_attachment_transition_state.h"
namespace Neon
{
	namespace Graphics
	{
		struct Vertex {
			Vertex(float x, float y, float z, float r, float g, float b, float a) : pos(x, y, z), color(r, g, b, z) {}
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT4 color;
		};

		DX12GraphicsContext::DX12GraphicsContext(Core::Window* _window)
			: GraphicsContext(_window)
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


			// Setup the swapchain desc
			SwapchainDescriptor swapchainDesc = {};
			swapchainDesc.Name			  = "Main-Swapchain";
			swapchainDesc.Width			  = _window->GetWindowWidth();
			swapchainDesc.Height		  = _window->GetWindowHeight();
			swapchainDesc.BackBufferCount = 3;
			swapchainDesc.Window		  = _window;
		
			m_Swapchain = Swapchain::Create(m_CommandQueue, &swapchainDesc);

		//	// Desc for backbuffer
		//	DXGI_MODE_DESC backBufferDesc = {}; 
		//	backBufferDesc.Width		  = _window->GetWindowWidth(); 
		//	backBufferDesc.Height		  = _window->GetWindowHeight();
		//	backBufferDesc.Format		  = DXGI_FORMAT_R8G8B8A8_UNORM; 
		//
		//	DXGI_SAMPLE_DESC sampleDesc = {};
		//	sampleDesc.Count			= 1;
		//
		//	// Describe and create the swap chain.
		//	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		//	swapChainDesc.BufferCount		   = frameBufferCount; 
		//	swapChainDesc.BufferDesc		   = backBufferDesc;
		//	swapChainDesc.BufferUsage		   = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
		//	swapChainDesc.SwapEffect		   = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		//	swapChainDesc.OutputWindow		   = (HWND)_window->GetNativeWindowHandle(); 
		//	swapChainDesc.SampleDesc		   = sampleDesc; 
		//	swapChainDesc.Windowed			   = true; 
		//
		//	IDXGISwapChain* tempSwapChain;
		//	dxgiFactory->CreateSwapChain(NEON_CAST(DX12CommandQueue*, m_CommandQueue)->m_CommandQueueObj, &swapChainDesc, &tempSwapChain);
		//
		//	m_SwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
		//	frameIndex = m_SwapChain->GetCurrentBackBufferIndex();


		//	// describe an rtv descriptor heap and create
		//	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		//	rtvHeapDesc.NumDescriptors			   = frameBufferCount; 
		//	rtvHeapDesc.Type					   = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; 
		//	rtvHeapDesc.Flags					   = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		//
		//	DX12_ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap)));


			// get the size of a descriptor in this heap 
		//	rtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		//
		//	// get a handle to the first descriptor in the descriptor heap
		//	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
		//
		//	// Create a RTV for each buffer
		//	for (int i = 0; i < frameBufferCount; i++)
		//	{
		//		DX12_ThrowIfFailed(NEON_CAST(DX12Swapchain*, m_Swapchain)->m_SwapChainObj->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])));
		//
		//		// Create rendertarget and bind swapcahin[i] to rtv
		//		m_Device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);
		//
		//		rtvHandle.Offset(1, rtvDescriptorSize);
		//	}

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

			// Setup GraphicsPipeline Descriptor
			GraphicsPipelineDescriptor pipelineDesc = {};
			pipelineDesc.Name		  = "Main-GraphicsPipeline";
			pipelineDesc.ImageWidth   = _window->GetWindowWidth();
			pipelineDesc.ImageHeight  = _window->GetWindowHeight();
			pipelineDesc.InputLayout  = reflection.Layout;
			pipelineDesc.Shader		  = shader;

			// Create the Graphics Pipeline
			m_GraphicsPipeline = GraphicsPipeline::Create(&pipelineDesc);


			// Framebuffer
			FramebufferLayout framebufferLayout = { FramebufferAttachmentType::NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT, };

			// RenderpassDescriptor
			RenderpassDescriptor renderpassDesc = {};
			renderpassDesc.Name   = "Main-Renderpass";
			renderpassDesc.Layout = FramebufferLayout({ NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT, });

			m_Renderpass = Renderpass::Create(&renderpassDesc);

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
				m_Framebuffer[i] = Framebuffer::Create(&framebufferDesc, m_Renderpass);

				// Get swapchain image and add to framebuffer
				m_Framebuffer[i]->AddAttachment(m_Swapchain->GetFramebufferAttachment(i));
				m_Framebuffer[i]->AddAttachment(m_DepthAttachment);
			}

		//	// create a depth stencil descriptor heap 
		//	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		//	dsvHeapDesc.NumDescriptors = 1;
		//	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		//	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		//	DX12_ThrowIfFailed(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsDescriptorHeap)));
		//
		//
		//	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
		//	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		//	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
		//
		//	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
		//	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
		//	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
		//	depthOptimizedClearValue.DepthStencil.Stencil = 0;
		//
		//	m_Device->CreateCommittedResource(
		//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		//		D3D12_HEAP_FLAG_NONE,
		//		&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, _window->GetWindowWidth(), _window->GetWindowHeight(), 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		//		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		//		&depthOptimizedClearValue,
		//		IID_PPV_ARGS(&depthStencilBuffer)
		//	);
		//	dsDescriptorHeap->SetName(L"Depth/Stencil Resource Heap");
		//
		//	m_Device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

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
			m_Viewport = Viewport::Create(0, 0, _window->GetWindowWidth(), _window->GetWindowHeight());
			m_Scissor  = Scissor::Create(0, 0, _window->GetWindowWidth(), _window->GetWindowHeight());

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
			{
				HRESULT hr;

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
				const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
				m_CommandBuffers[frameIndex]->ClearFrameBuffer(m_Framebuffer[frameIndex], clearColor, 0, 0, 0);

				m_CommandBuffers[frameIndex]->BeginRenderpass(m_Renderpass, m_Framebuffer[frameIndex]);

				// draw triangle
				m_CommandBuffers[frameIndex]->SetViewport(m_Viewport);
				m_CommandBuffers[frameIndex]->SetScissor(m_Scissor);
				m_CommandBuffers[frameIndex]->SetTopology(Topology::NEON_TOPOLOGY_TRIANGLE_LIST);

				m_CommandBuffers[frameIndex]->SetVertexBuffer(m_VertexBuffer);
				m_CommandBuffers[frameIndex]->SetIndexBuffer(m_IndexBuffer);

				m_CommandBuffers[frameIndex]->DrawIndexed(6, 0, 0);
				m_CommandBuffers[frameIndex]->DrawIndexed(6, 0, 4);

				// Transition state back
				m_CommandBuffers[frameIndex]->TransitionFramebufferAttachment(m_Framebuffer[frameIndex]->GetAttachment(0), NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER, NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT);

				m_CommandBuffers[frameIndex]->EndRenderpass(m_Renderpass);

				m_CommandBuffers[frameIndex]->EndRecording();
			}

			{
				// Execure commandBuffer
				m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[frameIndex], m_SubmitFence);

				m_SubmitFence->WaitForFence();
				m_SubmitFence->Reset();

				// present the current backbuffer
				m_Swapchain->Present(m_CommandQueue, false);
			}
		}
	}
}