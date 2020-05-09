#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		FramebufferAttachment::FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor)
			: m_Type(_framebufferAttachmentDescriptor->Type)
		{

		}
	}
}