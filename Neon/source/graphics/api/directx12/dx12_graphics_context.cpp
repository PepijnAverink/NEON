#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./core/window/window.h"

#include <DX12/d3dx12.h>
#include "./graphics/api/directx12/dx12_error.h"

#include "./graphics/api/directx12/resources/shader/dx12_shader.h"
#include "./graphics/api/directx12/pipeline/inputLayout/dx12_input_layout.h"

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

			int adapterIndex  = 0;
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
			queueDesc.Name		 = "Main-CommandQueue";
			queueDesc.QueueIndex = 0;
			queueDesc.Type		 = CommandQueueType::NEON_COMMAND_QUEUE_TYPE_DIRECT;

			// Create the queue itself
			m_CommandQueue		 = CommandQueue::Create(&queueDesc);
		
			// Setup pool descriptor
			CommandPoolDescriptor poolDesc = {};
			poolDesc.Name		 = "Main-CommandPool";
			poolDesc.Type		 = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
			
			// Create command pool
			m_CommandPool		 = CommandPool::Create(&poolDesc);
			
			// Setup command buffer
			for (int i = 0; i < frameBufferCount; ++i)
			{
				// Setup commad buffer descriptor
				CommandBufferDescriptor commandBufferDesc = {};
				commandBufferDesc.Name		  = "Main-CommandBuffer: " + i;
				commandBufferDesc.Type		  = CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT;
				commandBufferDesc.CommandPool = m_CommandPool;
		
				m_CommandBuffers[i]			  = CommandBuffer::Create(&commandBufferDesc);
			}
		
			// Use this commandBuffer for setup
			m_CommandBuffers[0]->StartRecording();

			// Setup fences
			FenceDescriptor fenceDesc = {};
			fenceDesc.Name = "AquireFence";
			m_AcuireFence  = Fence::Create(&fenceDesc);

			fenceDesc.Name = "SubmitFence";
			m_SubmitFence  = Fence::Create(&fenceDesc);

			// Desc for backbuffer
			DXGI_MODE_DESC backBufferDesc = {}; 
			backBufferDesc.Width		  = _window->GetWindowWidth(); 
			backBufferDesc.Height		  = _window->GetWindowHeight();
			backBufferDesc.Format		  = DXGI_FORMAT_R8G8B8A8_UNORM; 

			DXGI_SAMPLE_DESC sampleDesc = {};
			sampleDesc.Count			= 1;

			// Describe and create the swap chain.
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			swapChainDesc.BufferCount		   = frameBufferCount; 
			swapChainDesc.BufferDesc		   = backBufferDesc;
			swapChainDesc.BufferUsage		   = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
			swapChainDesc.SwapEffect		   = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.OutputWindow		   = (HWND)_window->GetNativeWindowHandle(); 
			swapChainDesc.SampleDesc		   = sampleDesc; 
			swapChainDesc.Windowed			   = true; 

			IDXGISwapChain* tempSwapChain;
			dxgiFactory->CreateSwapChain(NEON_CAST(DX12CommandQueue*, m_CommandQueue)->m_CommandQueueObj, &swapChainDesc, &tempSwapChain);

			m_SwapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
			frameIndex = m_SwapChain->GetCurrentBackBufferIndex();


			// describe an rtv descriptor heap and create
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors			   = frameBufferCount; 
			rtvHeapDesc.Type					   = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; 
			rtvHeapDesc.Flags					   = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			DX12_ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap)));


			// get the size of a descriptor in this heap 
			rtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			// get a handle to the first descriptor in the descriptor heap
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each buffer
			for (int i = 0; i < frameBufferCount; i++)
			{
				DX12_ThrowIfFailed(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])));

				// Create rendertarget and bind swapcahin[i] to rtv
				m_Device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);

				rtvHandle.Offset(1, rtvDescriptorSize);
			}

			// create root signature
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ID3DBlob* signature;
			DX12_ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, nullptr));
			DX12_ThrowIfFailed(m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));



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

			// fill out a shader bytecode structure
			D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
			vertexShaderBytecode.BytecodeLength		   = NEON_CAST(DX12Shader*, shader)->m_VertexShaderBytes->GetBufferSize();
			vertexShaderBytecode.pShaderBytecode	   = NEON_CAST(DX12Shader*, shader)->m_VertexShaderBytes->GetBufferPointer();

			// fill out shader bytecode structure for pixel shader
			D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
			pixelShaderBytecode.BytecodeLength		  = NEON_CAST(DX12Shader*, shader)->m_FragmentShaderBytes->GetBufferSize();
			pixelShaderBytecode.pShaderBytecode		  = NEON_CAST(DX12Shader*, shader)->m_FragmentShaderBytes->GetBufferPointer();

			// create input layout
			D3D12_INPUT_ELEMENT_DESC inputLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
			};

			std::vector<D3D12_INPUT_ELEMENT_DESC> ip;
			GetDX12InputLayout(ip, &reflection.Layout);

			// fill out an input layout description structure
			D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
			inputLayoutDesc.NumElements				= reflection.Layout.GetElementCount();
			inputLayoutDesc.pInputElementDescs		= ip.data();

			// create a pipeline state object (PSO)
			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; 
			psoDesc.InputLayout						   = inputLayoutDesc;
			psoDesc.pRootSignature					   = rootSignature;
			psoDesc.VS								   = vertexShaderBytecode;
			psoDesc.PS								   = pixelShaderBytecode; 
			psoDesc.PrimitiveTopologyType			   = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			psoDesc.RTVFormats[0]					   = DXGI_FORMAT_R8G8B8A8_UNORM; 
			psoDesc.SampleDesc						   = sampleDesc;
			psoDesc.SampleMask						   = 0xffffffff;
			psoDesc.RasterizerState					   = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			psoDesc.BlendState						   = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			psoDesc.NumRenderTargets				   = 1;
			psoDesc.DepthStencilState				   = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

			// create the pso
			DX12_ThrowIfFailed(m_Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineStateObject)));

			// create a depth stencil descriptor heap 
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DX12_ThrowIfFailed(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsDescriptorHeap)));


			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
			depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
			depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
			depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
			depthOptimizedClearValue.DepthStencil.Stencil = 0;

			m_Device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, _window->GetWindowWidth(), _window->GetWindowHeight(), 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&depthStencilBuffer)
			);
			dsDescriptorHeap->SetName(L"Depth/Stencil Resource Heap");

			m_Device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

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

			// Execute commandBuffer 
			m_CommandBuffers[0]->EndRecording();
			m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[0], m_AcuireFence);

			// Fill out the Viewport
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = _window->GetWindowWidth();
			viewport.Height = _window->GetWindowHeight();
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;

			// Fill out a scissor rect
			scissorRect.left = 0;
			scissorRect.top = 0;
			scissorRect.right = _window->GetWindowWidth();
			scissorRect.bottom = _window->GetWindowHeight();
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
				frameIndex = m_SwapChain->GetCurrentBackBufferIndex();
				hr = NEON_CAST(DX12CommandQueue*, m_CommandQueue)->m_CommandQueueObj->Signal(NEON_CAST(DX12Fence*, m_AcuireFence)->m_FenceObj, NEON_CAST(DX12Fence*, m_AcuireFence)->m_FenceValue);
				m_AcuireFence->WaitForFence();
				m_AcuireFence->Reset();

				// Reset Commandpool and start recording current commandBuffer
				m_CommandPool->Reset();
				m_CommandBuffers[frameIndex]->StartRecording();


				// Set pipeline state
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->SetPipelineState(pipelineStateObject);

				// Transition the backbuffer
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

				// Get handles
				CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);
				CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

				// Set render targets
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

				// Clear
				const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->ClearDepthStencilView(dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

				// draw triangle
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->SetGraphicsRootSignature(rootSignature);
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->RSSetViewports(1, &viewport); 
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->RSSetScissorRects(1, &scissorRect); 
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); 
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->IASetVertexBuffers(0, 1, &NEON_CAST(DX12VertexBuffer*, m_VertexBuffer)->m_VertexBufferView);
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->IASetIndexBuffer(&NEON_CAST(DX12IndexBuffer*, m_IndexBuffer)->m_IndexBufferView);
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->DrawIndexedInstanced(6, 1, 0, 0, 0);
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->DrawIndexedInstanced(6, 1, 0, 4, 0); // draw second quad

				// Transition state back
				NEON_CAST(DX12CommandBuffer*, m_CommandBuffers[frameIndex])->m_CommandListObj->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

				m_CommandBuffers[frameIndex]->EndRecording();
			}

			{
				// Execure commandBuffer
				m_CommandQueue->ExecuteCommandBuffer(m_CommandBuffers[frameIndex], m_SubmitFence);

				m_SubmitFence->WaitForFence();
				m_SubmitFence->Reset();

				// present the current backbuffer
				DX12_ThrowIfFailed(m_SwapChain->Present(0, 0));
			}
		}
	}
}