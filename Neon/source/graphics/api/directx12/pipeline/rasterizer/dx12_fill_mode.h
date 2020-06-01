#pragma once
#include "./graphics/pipeline/rasterizer/fill_mode.h"

#include <iostream>
#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		inline D3D12_FILL_MODE GetDX12FillMode(FillMode _mode)
		{
			switch (_mode)
			{
			case FillMode::NEON_FILL_MODE_FILL : return D3D12_FILL_MODE_SOLID;
			case FillMode::NEON_FILL_MODE_LINE:  return D3D12_FILL_MODE_WIREFRAME;
			}

			printf("[ERROR] GetDX12FillMode() - No valid FillMode was found for FillMode: %i", _mode);
			return D3D12_FILL_MODE_SOLID;
		}
	}
}