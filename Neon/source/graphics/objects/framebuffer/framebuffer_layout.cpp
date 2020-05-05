#include "./graphics/objects/framebuffer/framebuffer_layout.h"

namespace Neon
{
	namespace Graphics
	{
		FramebufferLayout::FramebufferLayout(const std::initializer_list<FramebufferAttachmentType>& _elements)
			: m_Elements(_elements)
		{ 	}

		void FramebufferLayout::AddElement(FramebufferAttachmentType _element)
		{
			m_Elements.push_back(_element);
		}
	}
}