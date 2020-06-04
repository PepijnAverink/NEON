#pragma once
#include "./graphics/objects/command/command_buffer_type.h"

#include <iostream>
#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		inline D3D12_COMMAND_LIST_TYPE GetDX12CommandBufferType(const CommandBufferType _type)
		{
			switch (_type)
			{
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT:   return D3D12_COMMAND_LIST_TYPE_DIRECT;
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_INDIRECT: return D3D12_COMMAND_LIST_TYPE_BUNDLE;
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_COMPUTE:  return D3D12_COMMAND_LIST_TYPE_COMPUTE;
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_TRANSFER: return D3D12_COMMAND_LIST_TYPE_COPY;
			}

			printf("[ERROR] GetDX12CommandBufferType() - No valid D3D12_COMMAND_LIST_TYPE was found for CommandBufferType: %i", _type);
			return D3D12_COMMAND_LIST_TYPE_DIRECT;
		}
	}
}