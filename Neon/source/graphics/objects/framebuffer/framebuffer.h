#pragma once
#include "./graphics/objects/framebuffer/framebuffer_descriptor.h"
#include "./graphics/objects/framebuffer/framebuffer_attachment.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class GraphicsPipeline;
		class Framebuffer
		{
		public:
			static Framebuffer* Create(const FramebufferDescriptor* _framebufferDescriptor, GraphicsPipeline* _graphicsPipeline);
			virtual ~Framebuffer() {}

			inline uint32_t GetAttachmentCount() const { return m_AttachmentCount; }

			FramebufferAttachment* GetAttachment(const int _attachmentIndex) const { return m_FramebufferAttachments[_attachmentIndex]; }
			inline bool IsDepthStencilActive() const { return m_DepthStencilActive; }

		protected:
			Framebuffer(const FramebufferDescriptor* _framebufferDescriptor);
			virtual void AddAttachment(FramebufferAttachment* _framebufferAttachment) = 0;

			uint32_t							m_AttachmentCount;
			std::vector<FramebufferAttachment*> m_FramebufferAttachments;

			bool m_DepthStencilActive;
		};
	}
}