#include "./graphics/pipeline/inputLayout/input_layout_element.h"

namespace Neon
{
	namespace Graphics
	{
		InputLayoutElement::InputLayoutElement(const std::string& _name, InputLayoutElementFormat _format, size_t _offset, bool _normalized)
			: Name(_name)
			, Format(_format)
			, Size(GetInputLayoutElementFormatSize(_format))
			, ElementCount(GetInputLayoutElementFormatElementCount(_format))
			, Offset(_offset)
			, Normalized(_normalized)
		{ }

		InputLayoutElement::InputLayoutElement(const std::string& _name, InputLayoutElementFormat _format, uint32_t _size, size_t _offset, bool _normalized)
			: Name(_name)
			, Format(_format)
			, Size(_size)
			, ElementCount(GetInputLayoutElementFormatElementCount(_format))
			, Offset(_offset)
			, Normalized(_normalized)
		{ }
	}
}