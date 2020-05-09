#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#include "./graphics/api/vulkan/objects/command/vk_command_buffer_type.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./graphics/api/directx12/resources/buffer/dx12_vertex_buffer.h"

#include "./graphics/objects/command_generic/viewport.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		// TODO:: Track commandBufferState for VK implementation
		VKCommandBuffer::VKCommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor)
			: CommandBuffer(_commandBufferDescriptor)
		{
			// Cast of commandPool
			auto commandPool = NEON_CAST(VKCommandPool*, _commandBufferDescriptor->CommandPool);
			commandPool->AddCommandBuffer(this);

			// mcbBeginInfo
			m_BeginInfo = {};
			m_BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			m_BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			// Alloc info
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool			= commandPool->m_CommandPoolObj;
			allocInfo.level					= GetCommandBufferLevelFromType(_commandBufferDescriptor->Type);
			allocInfo.commandBufferCount	= 1;
		
			// Create the actual command buffer
			VK_ThrowIfFailed(vkAllocateCommandBuffers(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &allocInfo, &m_CommandBufferObj));
		}

		void VKCommandBuffer::StartRecording() const
		{
			vkBeginCommandBuffer(m_CommandBufferObj, &m_BeginInfo);
		}

		void VKCommandBuffer::EndRecording() const
		{
		 	VK_ThrowIfFailed(vkEndCommandBuffer(m_CommandBufferObj));
		}

		void VKCommandBuffer::Reset() const
		{
			vkResetCommandBuffer(m_CommandBufferObj, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		}
		
		// Include the minDepth and maxDepth values
		void VKCommandBuffer::SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) const
		{

		}

		void VKCommandBuffer::SetVertexBuffer(VertexBuffer* _vertexBuffer) const
		{
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(m_CommandBufferObj, 0, 1, &NEON_CAST(VKVertexBuffer*, _vertexBuffer)->m_VertexBufferObj, offsets);
		}

		void VKCommandBuffer::SetIndexBuffer(IndexBuffer* _indexBuffer) const
		{

		}
		void VKCommandBuffer::SetTopology(const Topology _topology) const
		{

		}

		void VKCommandBuffer::SetTopology(TopologyState* _topologyState) const
		{

		}

		void VKCommandBuffer::SetViewport(Viewport * _viewport) const
		{

		}

		void VKCommandBuffer::SetScissor(Scissor * _scissor) const
		{

		}
		void VKCommandBuffer::ClearFrameBuffer(Framebuffer * _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const
		{
		}
		void VKCommandBuffer::BeginRenderpass(Renderpass * _renderpass, Framebuffer * _framebuffer) const
		{
		}
		void VKCommandBuffer::EndRenderpass(Renderpass * _renderpass) const
		{
		}
		void VKCommandBuffer::TransitionFramebufferAttachment(FramebufferAttachment * _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const
		{
		}
	}
}