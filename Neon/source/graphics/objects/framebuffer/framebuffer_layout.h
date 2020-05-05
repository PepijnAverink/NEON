#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_type.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class FramebufferLayout
		{
		public:
			FramebufferLayout() = default;
			FramebufferLayout(const std::initializer_list<FramebufferAttachmentType>& _elements);

			void AddElement(FramebufferAttachmentType _element);

			// Vector func
			std::vector<FramebufferAttachmentType>::iterator begin() { return m_Elements.begin(); }
			std::vector<FramebufferAttachmentType>::iterator end() { return m_Elements.end(); }
			std::vector<FramebufferAttachmentType>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<FramebufferAttachmentType>::const_iterator end()   const { return m_Elements.end(); }

			// Getters
			inline uint32_t							     GetAttachementCount() const { return (uint32_t)m_Elements.size(); }
			inline const std::vector<FramebufferAttachmentType>& GetElements() const { return m_Elements; }

		private:
			std::vector<FramebufferAttachmentType> m_Elements;
		};
	}
}