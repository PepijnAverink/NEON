#pragma once

#include <windows.h>
#include <exception>

namespace Neon
{
	namespace Graphics
	{
		inline void DX12_ThrowIfFailed(HRESULT _result)
		{
			if (_result != S_OK)
			{
				throw std::exception();
			}
		}
	}
}