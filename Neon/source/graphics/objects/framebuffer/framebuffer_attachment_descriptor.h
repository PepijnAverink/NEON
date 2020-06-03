#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_type.h"
#include "./graphics/objects/framebuffer/framebuffer_attachment_format.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class FramebufferAttachmentDescriptor
		{
		public:
			FramebufferAttachmentDescriptor() = default;

			std::string					Name;
			FramebufferAttachmentType	Type;
			FramebufferAttachmentFormat	Format;

			int							Width;
			int							Height;
		};
	}
}