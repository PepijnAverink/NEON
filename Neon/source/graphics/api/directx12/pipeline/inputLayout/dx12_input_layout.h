#pragma once
#include "./graphics/api/directx12/pipeline/inputLayout/dx12_input_layout_element_format.h"
#include "./graphics/pipeline/inputLayout/input_layout.h"
#include "./utilities/string_utils.h"

#include <d3d12.h>
#include <vector>

namespace Neon
{
	namespace Graphics
	{
		inline void GetDX12InputLayout(std::vector<D3D12_INPUT_ELEMENT_DESC>& _elements, const InputLayout* _inputLayout)
		{
			// Setup 
			const std::vector<InputLayoutElement>& inputElements = _inputLayout->GetElements();
			uint32_t parameterCount = _inputLayout->GetElementCount();

			// Loop over all paramters
			for (int i = 0; i < parameterCount; i++)
			{
				D3D12_INPUT_ELEMENT_DESC el;
				el.SemanticName				= inputElements[i].Name.c_str();
				el.SemanticIndex			= 0;
				el.Format				    = GetDXGIFormatFromInputLayoutElementFormat(inputElements[i].Format);
				
				el.AlignedByteOffset		= D3D12_APPEND_ALIGNED_ELEMENT;
				el.InputSlot				= 0;
				el.InputSlotClass			= D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
				el.InstanceDataStepRate	= 0;

				_elements.push_back(el);
			}
		}
	}
}