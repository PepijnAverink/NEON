#pragma once
#include "./graphics/objects/framebuffer/framebuffer_descriptor.h"
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class Renderpass;
		class Framebuffer
		{
		public:
			static Framebuffer* Create(const FramebufferDescriptor* _framebufferDescriptor, Renderpass* _renderpass);

			virtual void AddAttachment(FramebufferAttachment* _framebufferAttachment) = 0;

			inline uint32_t GetAttachmentCount() const { return m_AttachmentCount; }

			FramebufferAttachment* GetAttachment(const int _attachmentIndex) const { return m_FramebufferAttachments[_attachmentIndex]; }
			inline bool IsDepthStencilActive() const { return m_DepthStencilActive; }

		protected:
			Framebuffer(const FramebufferDescriptor* _framebufferDescriptor);

			uint32_t							m_AttachmentCount;
			std::vector<FramebufferAttachment*> m_FramebufferAttachments;

			bool m_DepthStencilActive;
		};
	}
}