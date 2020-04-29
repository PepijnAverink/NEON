#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#include "./graphics/api/vulkan/objects/command/vk_command_buffer_type.h"
#include "./graphics/api/vulkan/vk_error.h"

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
		void VKCommandBuffer::SetViewport(const Viewport _viewport) const
		{
			VkViewport vp = VkViewport({ _viewport.BottomX, _viewport.BottomY, _viewport.Width, _viewport.Height, 0, 1 });
			vkCmdSetViewport(m_CommandBufferObj, 0, 1, &vp);
		}

		void VKCommandBuffer::SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) const
		{

		}

		void VKCommandBuffer::SetVertexBuffer(VertexBuffer* _vertexBuffer) const
		{

		}

		void VKCommandBuffer::SetIndexBuffer(IndexBuffer* _indexBuffer) const
		{

		}
	}
}