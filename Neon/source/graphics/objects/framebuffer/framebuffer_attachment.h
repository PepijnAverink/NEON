#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_descriptor.h"

namespace Neon
{
	namespace Graphics
	{
		class FramebufferAttachment
		{
		public:
			static FramebufferAttachment* Create(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);

			inline FramebufferAttachmentType GetType() const { return m_Type; }

		protected:
			FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);

			FramebufferAttachmentType m_Type;

		};
	}
}