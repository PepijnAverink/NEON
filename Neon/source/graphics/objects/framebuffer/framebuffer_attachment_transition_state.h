#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum FramebufferAttachmentTransitionState
		{
			NEON_FRAMEBUFFER_TRANSITION_STATE_NONE    = 0x00,
			NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER  = 0x01,
			NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT = 0x02,
		};
	}
}