#include "./graphics/api/directx11/objects/command/dx11_command_pool.h"

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

		}
	}
}