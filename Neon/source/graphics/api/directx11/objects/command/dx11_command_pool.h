#pragma once
#include "./graphics/objects/command/command_pool.h"

namespace Neon
{
	namespace Graphics
	{
		class DX11CommandPool : public CommandPool
		{
		public:
			DX11CommandPool(CommandPoolDescriptor* _commandPoolDescriptor);
			virtual ~DX11CommandPool();

			virtual void Reset() const override;

		private:
		};
	}
}