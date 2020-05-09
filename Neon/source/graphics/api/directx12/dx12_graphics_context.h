#pragma once
#include "./graphics/graphics_context.h"

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <string>

// Graphics abstraction
#include "./utilities/casting/casting_helper.h"
#include "./graphics/api/directx12/objects/sync/dx12_fence.h"
#include "./graphics/api/directx12/objects/command/dx12_command_pool.h"
#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"
#include "./graphics/api/directx12/objects/command/dx12_command_queue.h"

#include "./graphics/api/directx12/resources/buffer/dx12_vertex_buffer.h"
#include "./graphics/api/directx12/resources/buffer/dx12_index_buffer.h"

#include "./graphics/api/directx12/pipeline/dx12_graphics_pipeline.h"

#include "./graphics/api/directx12/objects/renderpass/dx12_renderpass.h"
#include "./graphics/api/directx12/objects/framebuffer/dx12_framebuffer.h"

#include "./graphics/objects/command_generic/viewport.h"
#include "./graphics/objects/command_generic/scissor.h"

#include "./graphics/api/directx12/objects/swapchain/dx12_swapchain.h"


namespace Neon
{
	namespace Graphics
	{
		class DX12GraphicsContext final : public GraphicsContext
		{
		public:
			DX12GraphicsContext(Core::Window* _window);
			virtual ~DX12GraphicsContext();

			virtual bool Initialize() override;
			virtual bool Terminate()  override;

			virtual void Present() override;

			// Get's a Dx12 instance
			static DX12GraphicsContext* GetInstance() { return (DX12GraphicsContext*)GetGraphicsContext(); }


			inline ID3D12Device* GetGraphicsDevice() const { return m_Device; }

		private:

			ID3D12Device* m_Device;
		
			int frameIndex;

		//	ID3D12Resource* depthStencilBuffer;
		//	ID3D12DescriptorHeap* dsDescriptorHeap;

			FramebufferAttachment* m_DepthAttachment;

			// Own abstraction
			Viewport*		  m_Viewport;
			Scissor*		  m_Scissor;

			CommandQueue*     m_CommandQueue;
						      
			CommandPool*      m_CommandPool;
			CommandBuffer*    m_CommandBuffers[frameBufferCount];
						      
			Fence*		      m_SubmitFence;
			Fence*		      m_AcuireFence;

			GraphicsPipeline* m_GraphicsPipeline;

			VertexBuffer*     m_VertexBuffer;
			IndexBuffer*      m_IndexBuffer;

			Swapchain*		  m_Swapchain;

			Renderpass*		  m_Renderpass;
			Framebuffer*	  m_Framebuffer[frameBufferCount];
		};
	}
}