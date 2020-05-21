#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		class VKFramebufferAttachment : public FramebufferAttachment
		{
		public:
			VKFramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);
		};
	}
}