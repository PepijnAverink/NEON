#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#include "./graphics/api/vulkan/objects/command/vk_command_buffer_type.h"
#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer_attachment.h"
#include "./graphics/api/vulkan/objects/framebuffer/vk_framebuffer_attachment_transition_state.h"
#include "./graphics/objects/framebuffer/framebuffer_clear_flags.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./graphics/api/vulkan/pipeline/vk_graphics_pipeline.h"

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
			// mcbBeginInfo
			m_BeginInfo = {};
			m_BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			m_BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

			// Alloc info
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool			= NEON_CAST(VKCommandPool*, _commandBufferDescriptor->CommandPool)->m_CommandPoolObj;
			allocInfo.level					= GetCommandBufferLevelFromType(_commandBufferDescriptor->Type);
			allocInfo.commandBufferCount	= 1;
		
			// Create the actual command buffer
			VK_ThrowIfFailed(vkAllocateCommandBuffers(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &allocInfo, &m_CommandBufferObj));
		}

		void VKCommandBuffer::StartRecording()
		{
			vkBeginCommandBuffer(m_CommandBufferObj, &m_BeginInfo);
		}

		void VKCommandBuffer::EndRecording()
		{
		 	VK_ThrowIfFailed(vkEndCommandBuffer(m_CommandBufferObj));
		}

		void VKCommandBuffer::Reset()
		{
			vkResetCommandBuffer(m_CommandBufferObj, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
		}
		
		// Include the minDepth and maxDepth values
		void VKCommandBuffer::SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline)
		{
			s_GraphicsPipeline = NEON_CAST(VKGraphicsPipeline*, _graphicsPipeline);
			vkCmdBindPipeline(m_CommandBufferObj, VK_PIPELINE_BIND_POINT_GRAPHICS, s_GraphicsPipeline->m_GraphicsPipeline);
		}

		void VKCommandBuffer::BindVertexBuffer(VertexBuffer* _vertexBuffer) const
		{
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(m_CommandBufferObj, 0, 1, &NEON_CAST(VKVertexBuffer*, _vertexBuffer)->m_VertexBufferObj, offsets);
		}

		void VKCommandBuffer::BindIndexBuffer(IndexBuffer* _indexBuffer) const
		{
			// Abstract the type
			vkCmdBindIndexBuffer(m_CommandBufferObj, NEON_CAST(VKIndexBuffer*, _indexBuffer)->m_IndexBufferObj, 0, VK_INDEX_TYPE_UINT16);

		}

		void VKCommandBuffer::BindComputeShader(ComputeShader* _computeShader)
		{
		}

		void VKCommandBuffer::BindComputeBuffer(ComputeBuffer* _computeBuffer)
		{
		}

		void VKCommandBuffer::DispatchCompute(const uint32_t _x, const uint32_t _y, const uint32_t _z)
		{
		}

		void VKCommandBuffer::BindTexture(Texture2D* _texture, uint32_t _bindPoint) const
		{

		}

		void VKCommandBuffer::BindTexture(FramebufferAttachment* _framebufferAttachment, uint32_t _bindPoint) const
		{

		}

		void VKCommandBuffer::SetViewport(Viewport* _viewport) const
		{

		}

		void VKCommandBuffer::SetScissor(Scissor* _scissor) const
		{

		}

		void VKCommandBuffer::ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const
		{
			// Note: contains value for each subresource range
			VkClearColorValue clearColor = {
				{ _color[0], _color[1], _color[2], _color[3], } // R, G, B, A
			};

			// TODO:: Take a look at this
			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subResourceRange.baseMipLevel = 0;
			subResourceRange.levelCount = 1;
			subResourceRange.baseArrayLayer = 0;
			subResourceRange.layerCount = 1;

			// TODO:: Support clearning of whole framebuffer
			if (_flags & NEON_CLEAR_COLOR_BIT)
				vkCmdClearColorImage(m_CommandBufferObj, NEON_CAST(VKFramebufferAttachment*, _framebuffer->GetAttachment(0))->m_Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subResourceRange);
		}

		void VKCommandBuffer::BeginRenderpass(Framebuffer* _framebuffer) const
		{
			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass  = s_GraphicsPipeline->m_RenderPass;
			renderPassInfo.framebuffer = NEON_CAST(VKFramebuffer*, _framebuffer)->m_FramebufferObj;
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = { 1280, 720 }; // TODO:: ABstract this size

			vkCmdBeginRenderPass(m_CommandBufferObj, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		}

		void VKCommandBuffer::EndRenderpass() const
		{
			vkCmdEndRenderPass(m_CommandBufferObj);
		}

		void VKCommandBuffer::TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const
		{
			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			subResourceRange.baseMipLevel	= 0;
			subResourceRange.levelCount		= 1;
			subResourceRange.baseArrayLayer = 0;
			subResourceRange.layerCount		= 1;

			// Change layout of image to be optimal for clearing
			VkImageMemoryBarrier presentToClearBarrier = {};
			presentToClearBarrier.sType					= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			presentToClearBarrier.srcAccessMask			= VK_ACCESS_MEMORY_READ_BIT;
			presentToClearBarrier.dstAccessMask			= VK_ACCESS_TRANSFER_WRITE_BIT;
			presentToClearBarrier.oldLayout				= VK_IMAGE_LAYOUT_UNDEFINED;
			presentToClearBarrier.newLayout				= GetVKFramebufferTransitionState(_toState);
			presentToClearBarrier.srcQueueFamilyIndex	= VKGraphicsContext::GetInstance()->GetQueueFamilyIDGraphics(); // TODO:: abstract this
			presentToClearBarrier.dstQueueFamilyIndex	= VKGraphicsContext::GetInstance()->GetQueueFamilyIDGraphics();
			presentToClearBarrier.image					= NEON_CAST(VKFramebufferAttachment*, _framebufferAttachment)->m_Image;
			presentToClearBarrier.subresourceRange		= subResourceRange;

			vkCmdPipelineBarrier(m_CommandBufferObj, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &presentToClearBarrier);
		}

		void VKCommandBuffer::DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const
		{
			vkCmdDrawIndexed(m_CommandBufferObj, _indexCount, 1, _indexOffset, _vertexOffset, 0);
		}
	}
}