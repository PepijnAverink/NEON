#pragma once
#include "./graphics/pipeline/inputLayout/input_layout_element_format.h"

#include <string>

namespace Neon
{
	namespace Graphics
	{
		class InputLayoutElement
		{
		public:
			InputLayoutElement() = default;
			InputLayoutElement(const std::string& _name, InputLayoutElementFormat _format, size_t _offset, bool _normalized = false);
			InputLayoutElement(const std::string& _name, InputLayoutElementFormat _format, uint32_t _size, size_t _offset, bool _normalized = false);

			std::string					Name;
			InputLayoutElementFormat	Format;
			uint32_t					Size;
			uint32_t					ElementCount;
			size_t						Offset;
			bool						Normalized;
		};
	}
}