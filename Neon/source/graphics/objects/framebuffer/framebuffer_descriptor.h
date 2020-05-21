#pragma once
#include "./graphics/objects/framebuffer/framebuffer_layout.h"

#include <string>
namespace Neon
{
	namespace Graphics
	{
		class FramebufferAttachment;
		class FramebufferDescriptor
		{
		public:
			FramebufferDescriptor() = default;

			std::string		  Name;

			int				  Width;
			int				  Height;

			FramebufferAttachment** Attachments;
			uint32_t		        AttachmentCount;
			bool			        DepthAttachment;

			FramebufferLayout       Layout;
		};
	}
}