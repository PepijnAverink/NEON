#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdexcept>

namespace Neon
{
	namespace Graphics
	{
		// From DXSampleHelper.h 
		// Source: https://github.com/Microsoft/DirectX-Graphics-Samples
		inline void DX11_ThrowIfFailed(HRESULT hr)
		{
			if (FAILED(hr))
			{
				throw std::exception();
			}
		}
	}
}