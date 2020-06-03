#include "./graphics/objects/framebuffer/framebuffer_attachment_ds.h"

namespace Neon
{
	namespace Graphics
	{
		FramebufferAttachment* FramebufferAttachmentDS::Create(const FramebufferAttachmentDescriptor * _framebufferAttachmentDescriptor)
		{
			return nullptr;
		}

		FramebufferAttachmentDS::FramebufferAttachmentDS(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: FramebufferAttachment(_framebufferAttachmentDescriptor)
		{ }
	}
}