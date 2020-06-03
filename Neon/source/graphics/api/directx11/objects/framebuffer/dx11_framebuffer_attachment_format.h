#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_format.h"

#include <dxgiformat.h>

namespace Neon
{
	namespace Graphics
	{
		inline DXGI_FORMAT GetDXGIFramebufferAttachmentFormat(const FramebufferAttachmentFormat _format)
		{
			switch (_format)
			{
				case FramebufferAttachmentFormat::NEON_FRAMEBUFFER_ATTACHMENT_FORMAT_D32_FLOAT:			return DXGI_FORMAT_D32_FLOAT;
				case FramebufferAttachmentFormat::NEON_FRAMEBUFFER_ATTACHMENT_FORMAT_D24_UNORM_S8_UINT: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			}
		}
	}
}