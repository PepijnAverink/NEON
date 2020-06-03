#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		class FramebufferAttachmentDS : public FramebufferAttachment
		{
		public:
			static FramebufferAttachment* Create(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);
			virtual ~FramebufferAttachmentDS() {}

			inline FramebufferAttachmentType GetType() const { return m_Type; }

		protected:
			FramebufferAttachmentDS(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);

			FramebufferAttachmentType m_Type;
		};
	}
}