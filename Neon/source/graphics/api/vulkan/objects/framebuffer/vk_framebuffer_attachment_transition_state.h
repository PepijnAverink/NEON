#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_transition_state.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		VkImageLayout GetVKFramebufferTransitionState(const FramebufferAttachmentTransitionState _framebufferTransitionState)
		{
			switch (_framebufferTransitionState)
			{
				case FramebufferAttachmentTransitionState::NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER:  return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				case FramebufferAttachmentTransitionState::NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT: return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}

			printf("[ERROR] GetVKFramebufferTransitionState() - No valid instance was found!");
			return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}
}