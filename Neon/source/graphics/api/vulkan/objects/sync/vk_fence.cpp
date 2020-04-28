#include "./graphics/api/vulkan/objects/sync/vk_fence.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		VKFence::VKFence(const FenceDescriptor* _fenceDestriptor)
			: Fence(_fenceDestriptor)
		{
			VkFenceCreateInfo fenceInfo = { };
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			vkCreateFence(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &fenceInfo, nullptr, &m_FenceObj);
		}

		VKFence::~VKFence()
		{
			vkDestroyFence(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_FenceObj, nullptr);
		}

		FenceReturn VKFence::WaitForFence()
		{
			VkResult result = vkWaitForFences(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), 1, &m_FenceObj, VK_TRUE, m_TimeOut);
			if (result == VK_SUCCESS) {
				return FenceReturn::NEON_FENCE_RETURN_SUCCESS;
			}
			else if (result == VK_NOT_READY)
				return FenceReturn::NEON_FENCE_RETURN_TIMEOUT;
		}

		const FenceStatus VKFence::GetFenceStatus()
		{
			VkResult result = vkGetFenceStatus(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_FenceObj);
			if (result == VK_SUCCESS)
				m_Status = FenceStatus::NEON_FENCE_STATUS_READY;
			else if (result == VK_NOT_READY)
				m_Status = FenceStatus::NEON_FENCE_STATUS_NOT_READY;

			return m_Status;
		}

		void VKFence::Reset() const
		{
			vkResetFences(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), 1, &m_FenceObj);
		}
	}
}