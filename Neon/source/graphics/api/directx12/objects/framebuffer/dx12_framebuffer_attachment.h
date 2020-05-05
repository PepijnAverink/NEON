#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

namespace Neon
{
	namespace Graphics
	{
		class DX12FramebufferAttachment : FramebufferAttachment
		{
		public:


		private:
			DX12FramebufferAttachment(const FramebufferAttachmentDescriptor* _framebufferAttachmentDescriptor);
		};
	}
}