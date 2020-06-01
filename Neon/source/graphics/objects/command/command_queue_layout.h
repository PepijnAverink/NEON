#pragma once
#include "./graphics/objects/command/command_queue_layout_element.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class CommandQueueLayout
		{
		public:
			CommandQueueLayout() = default;
			CommandQueueLayout(const std::initializer_list<CommandQueueLayoutElement>& _elements)
				: m_Elements(_elements)
			{ }

			void AddElement(CommandQueueLayoutElement _element) { m_Elements.push_back(_element); }

			// Vector func
			std::vector<CommandQueueLayoutElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<CommandQueueLayoutElement>::iterator end() { return m_Elements.end(); }
			std::vector<CommandQueueLayoutElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<CommandQueueLayoutElement>::const_iterator end()   const { return m_Elements.end(); }

			// Getters
			inline uint32_t									 GetElementCount()	const { return (uint32_t)m_Elements.size(); }
			inline const std::vector<CommandQueueLayoutElement>& GetElements() const { return m_Elements; }

		private:

			std::vector<CommandQueueLayoutElement> m_Elements;
		};
	}
}