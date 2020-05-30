#include "./graphics/api/vulkan/objects/command/vk_command_queue.h"
#include "./graphics/api/vulkan/objects/command/vk_command_queue_type.h"
#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
#include "./graphics/api/vulkan/objects/sync/vk_fence.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		VKCommandQueue::VKCommandQueue(CommandQueueDescriptor* _commandQueueDescriptor)
			: CommandQueue(_commandQueueDescriptor)
		{
			// Query the device for the wanted queue
			vkGetDeviceQueue(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), GetFamilyQueueIndexFromType(m_Type), m_QueueIndex, &m_CommandQueueObj);
		}

		void VKCommandQueue::ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const
		{
			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores	  = VK_NULL_HANDLE;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = VK_NULL_HANDLE;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &NEON_CAST(VKCommandBuffer*, _commandBuffer)->m_CommandBufferObj;

			// Submits the queue
			VK_ThrowIfFailed(vkQueueSubmit(m_CommandQueueObj, 1, &submitInfo, NEON_CAST(VKFence*, _signalFence)->m_FenceObj));
		
		}
	}
}