#pragma once
#include "./graphics/pipeline/rasterizer/cull_mode.h"

#include <iostream>
#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		inline D3D12_CULL_MODE GetDX12FillMode(CullMode _mode)
		{
			switch (_mode)
			{
				case CullMode::NEON_CULL_MODE_FRONT: return D3D12_CULL_MODE_FRONT;
				case CullMode::NEON_CULL_MODE_BACK:  return D3D12_CULL_MODE_BACK;
			}

			printf("[ERROR] GetDX12FillMode() - No valid CullMode was found for CullMode: %i", _mode);
			return D3D12_CULL_MODE_BACK;
		}
	}
}