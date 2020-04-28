#include "./graphics/pipeline/inputLayout/input_layout.h"

namespace Neon
{
	namespace Graphics
	{
		InputLayout::InputLayout(const std::initializer_list<InputLayoutElement>& _elements)
			: m_Elements(_elements)
		{
			CalculateOffsetsAndStride();
		}

		void InputLayout::AddElement(InputLayoutElement _element)
		{
			m_Elements.push_back(_element);
			CalculateOffsetsAndStride();
		}

		void InputLayout::CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	}
}