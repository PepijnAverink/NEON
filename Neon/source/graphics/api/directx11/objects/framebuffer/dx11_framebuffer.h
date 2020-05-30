#pragma once
#include "./graphics/objects/framebuffer/framebuffer.h"

#include <d3d11.h>

namespace Neon
{
	namespace Graphics
	{
		class DX11Framebuffer : public Framebuffer
		{
		public:
			DX11Framebuffer(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline);

		private:
			virtual void AddAttachment(FramebufferAttachment* _framebufferAttachment) override;

		};
	}
}