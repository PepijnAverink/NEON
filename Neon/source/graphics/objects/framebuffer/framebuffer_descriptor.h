#pragma once
#include "./graphics/objects/framebuffer/framebuffer_layout.h"

#include <string>
namespace Neon
{
	namespace Graphics
	{
		class FramebufferDescriptor
		{
		public:
			FramebufferDescriptor() = default;

			std::string		  Name;

			int				  Width;
			int				  Height;

			uint32_t		  AttachmentCount;
			bool			  DepthAttachment;

			FramebufferLayout Layout;
		};
	}
}