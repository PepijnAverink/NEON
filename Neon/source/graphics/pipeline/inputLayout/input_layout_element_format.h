#pragma once
#include <stdint.h>
#include <iostream>

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// InputLayoutElementFormat enum
		// An enum describing the supported formats for the input layout.
		//
		// SINT - SIGNED   INTEGER
		// UINT - UNSIGNED INTEGER
		// ------------------------------------------
		enum InputLayoutElementFormat
		{
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_NONE				= 0x00,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_FLOAT	= 0x01,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_FLOAT	= 0x02,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_FLOAT		= 0x03,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_FLOAT			= 0x04,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_SINT	= 0x05,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_SINT		= 0x06,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_SINT		= 0x07,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_SINT			= 0x08,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_UINT	= 0x09,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_UINT		= 0x0A,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_UINT		= 0x0B,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_UINT			= 0x0C,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_SINT	= 0x0D,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_SINT		= 0x0E,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_SINT		= 0x0F,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_SINT			= 0x10,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_UINT	= 0x11,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_UINT		= 0x12,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_UINT		= 0x13,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_UINT			= 0x14,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_SINT		= 0x15,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_SINT		= 0x16,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_SINT			= 0x17,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_SINT			= 0x18,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_UINT		= 0x19,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_UINT		= 0x1A,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_UINT			= 0x1B,
			NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_UINT			= 0x1C,
		};

		// ------------------------------------------
		// GetInputLayoutElementFormatSize function
		// Retrieves the size of InputLayoutElementFormat given is as a parameter
		// ------------------------------------------
		inline uint32_t GetInputLayoutElementFormatSize(const InputLayoutElementFormat _inputLayoutElementFormat)
		{
			switch (_inputLayoutElementFormat)
			{
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_FLOAT:	return 4 * sizeof(float);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_SINT:	return 4 * sizeof(int);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_UINT:	return 4 * sizeof(unsigned);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_SINT:	return 4 * sizeof(short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_UINT:	return 4 * sizeof(unsigned short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_SINT:		return 4 * sizeof(char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_UINT:		return 4 * sizeof(unsigned char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_FLOAT:	return 3 * sizeof(float);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_SINT:		return 3 * sizeof(int);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_UINT:		return 3 * sizeof(unsigned);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_SINT:		return 3 * sizeof(short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_UINT:		return 3 * sizeof(unsigned short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_SINT:		return 3 * sizeof(char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_UINT:		return 3 * sizeof(unsigned char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_FLOAT:		return 2 * sizeof(float);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_SINT:		return 2 * sizeof(int);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_UINT:		return 2 * sizeof(unsigned);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_SINT:		return 2 * sizeof(short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_UINT:		return 2 * sizeof(unsigned short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_SINT:			return 2 * sizeof(char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_UINT:			return 2 * sizeof(unsigned char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_FLOAT:			return sizeof(float);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_SINT:			return sizeof(int);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_UINT:			return sizeof(unsigned);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_SINT:			return sizeof(short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_UINT:			return sizeof(unsigned short);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_SINT:			return sizeof(char);
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_UINT:			return sizeof(unsigned char);
			}

			printf("[WARNING] GetInputLayoutElementFormatSize() - The size of the InputLayoutElementFormat could not be retrieved, format: %i", _inputLayoutElementFormat);
			return 0;
		}

		// ------------------------------------------
		// GetInputLayoutElementFormatElementCount function
		// Returns the number of elements found in the InputLayoutElementFormat given in as a parameter
		// ------------------------------------------
		inline uint32_t GetInputLayoutElementFormatElementCount(const InputLayoutElementFormat _inputLayoutElementFormat)
		{
			switch (_inputLayoutElementFormat)
			{
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_FLOAT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32A32_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16A16_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8A8_UINT: return 4;
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_FLOAT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32B32_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16B16_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8B8_UINT: return 3;
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_FLOAT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32G32_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16G16_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8G8_UINT: return 2;
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_FLOAT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R32_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R16_UINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_SINT:
				case InputLayoutElementFormat::NEON_INPUT_LAYOUT_ELEMENT_FORMAT_R8_UINT: return 1;
			}

			printf("[WARNING] GetInputLayoutElementFormatElementCount() - The count of the InputLayoutElementFormat could not be retrieved, format: %i", _inputLayoutElementFormat);
			return 0;
		}
	}
}