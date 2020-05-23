#pragma once

namespace Neon
{
	namespace Graphics
	{
		enum FramebufferAttachmentType
		{
			NEON_FRAMEBUFFER_ATTACHMENT_TYPE_NONE			= 0x00,
			NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_OUTPUT	= 0x01,
			NEON_FRAMEBUFFER_ATTACHMENT_TYPE_COLOR_INPUT	= 0x02,
			NEON_FRAMEBUFFER_ATTACHMENT_TYPE_DEPTH_STENCIL	= 0x03,
		};
	}
}