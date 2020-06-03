#include "./graphics/api/directx11/objects/command/dx11_command_pool.h"
#include "./graphics/api/directx11/objects/command/dx11_command_buffer.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX11CommandPool::DX11CommandPool(CommandPoolDescriptor* _commandPoolDescriptor)
			: CommandPool(_commandPoolDescriptor)
		{ }

		DX11CommandPool::~DX11CommandPool()
		{

		}

		void DX11CommandPool::Reset() const
		{
			// Reset all the CommandBuffers
			for (int i = 0; i < m_CommandBuffers.size(); i++)
				NEON_CAST(DX11CommandBuffer*, m_CommandBuffers[i])->Reset();
		}
	}
}