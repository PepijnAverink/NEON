#pragma once
#include "./graphics/pipeline/inputLayout/input_layout_element.h"

#include <vector>

namespace Neon
{
	namespace Graphics
	{
		class InputLayout
		{
		public:
			InputLayout() = default;
			InputLayout(const std::initializer_list<InputLayoutElement>& _elements);

			void AddElement(InputLayoutElement _element);

			// Vector func
			std::vector<InputLayoutElement>::iterator begin()	{ return m_Elements.begin(); }
			std::vector<InputLayoutElement>::iterator end()		{ return m_Elements.end();   }
			std::vector<InputLayoutElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<InputLayoutElement>::const_iterator end()   const { return m_Elements.end();   }

			// Getters
			inline uint32_t							GetElementCount()	const { return m_Elements.size(); }
			inline uint32_t									GetStride() const { return m_Stride; }
			inline const std::vector<InputLayoutElement>& GetElements() const { return m_Elements; }

		private:
			void CalculateOffsetsAndStride();

			std::vector<InputLayoutElement> m_Elements;
			uint32_t						m_Stride;
		};
	}
}