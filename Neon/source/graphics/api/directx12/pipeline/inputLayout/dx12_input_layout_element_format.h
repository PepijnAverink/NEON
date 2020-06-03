#pragma once
#include "./graphics/pipeline/inputLayout/input_layout_element_format.h"

#include <dxgiformat.h>
#include <d3d12shader.h>

namespace Neon
{
	namespace Graphics
	{
		inline DXGI_FORMAT GetDXGIFormatFromInputLayoutElementFormat(InputLayoutElementFormat _format)
		{
			switch (_format)
			{
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_NONE: return DXGI_FORMAT_UNKNOWN;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_FLOAT: return DXGI_FORMAT_R32G32B32_FLOAT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_FLOAT: return DXGI_FORMAT_R32G32_FLOAT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_FLOAT: return DXGI_FORMAT_R32_FLOAT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_SINT: return DXGI_FORMAT_R32G32B32A32_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_SINT: return DXGI_FORMAT_R32G32B32_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_SINT: return DXGI_FORMAT_R32G32_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_SINT: return DXGI_FORMAT_R32_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_UINT: return DXGI_FORMAT_R32G32B32A32_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_UINT: return DXGI_FORMAT_R32G32B32_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_UINT: return DXGI_FORMAT_R32G32_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_UINT: return DXGI_FORMAT_R32_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_SINT: return DXGI_FORMAT_R16G16B16A16_SINT;
				//	case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_SINT: return DXGI_FORMAT_R16G16B16_SINT;				// Unsupported?
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_SINT: return DXGI_FORMAT_R16G16_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_SINT: return DXGI_FORMAT_R16_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_UINT: return DXGI_FORMAT_R16G16B16A16_UINT;
				//	case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_UINT: return DXGI_FORMAT_R16G16B16_UINT;				// Unsupported?
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_UINT: return DXGI_FORMAT_R16G16_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_UINT: return DXGI_FORMAT_R16_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_SINT: return DXGI_FORMAT_R8G8B8A8_SINT;
				//	case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_SINT: return DXGI_FORMAT_R8G8B8_SINT;					// Unsupported?
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_SINT: return DXGI_FORMAT_R8G8_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_SINT: return DXGI_FORMAT_R8_SINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_UINT: return DXGI_FORMAT_R8G8B8A8_UINT;
				//	case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_UINT: return DXGI_FORMAT_R8G8B8_UINT;					// Unsupported?
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_UINT: return DXGI_FORMAT_R8G8_UINT;
			case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_UINT: return DXGI_FORMAT_R8_UINT;
			}

			printf("[ERROR] GetDXGIFormatFromInputLayoutElementFormat() - No valid DXGI format was found for fomat: %i", _format);
			return DXGI_FORMAT_UNKNOWN;
		}

		inline InputLayoutElementFormat GetInputLayoutElementFromatDXGI(const D3D12_SIGNATURE_PARAMETER_DESC& _paramDesc)
		{
			InputLayoutElementFormat format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_NONE;

			if (_paramDesc.Mask == 1)
			{
				switch (_paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_UINT;
					break;
				case D3D_REGISTER_COMPONENT_SINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_SINT;
					break;
				case D3D_REGISTER_COMPONENT_FLOAT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_FLOAT;
					break;
				}
			}
			else if (_paramDesc.Mask <= 3)
			{
				switch (_paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_UINT;
					break;
				case D3D_REGISTER_COMPONENT_SINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_SINT;
					break;
				case D3D_REGISTER_COMPONENT_FLOAT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_FLOAT;
					break;
				}
			}
			else if (_paramDesc.Mask <= 7)
			{
				switch (_paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_UINT;
					break;
				case D3D_REGISTER_COMPONENT_SINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_SINT;
					break;
				case D3D_REGISTER_COMPONENT_FLOAT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_FLOAT;
					break;
				}
			}
			else if (_paramDesc.Mask <= 15)
			{
				switch (_paramDesc.ComponentType)
				{
				case D3D_REGISTER_COMPONENT_UINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_UINT;
					break;
				case D3D_REGISTER_COMPONENT_SINT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_SINT;
					break;
				case D3D_REGISTER_COMPONENT_FLOAT32:
					format = InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_FLOAT;
					break;
				}
			}

			return format;
		}
	}
}