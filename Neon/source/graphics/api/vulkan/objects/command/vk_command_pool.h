#pragma once
#include "./graphics/objects/command/command_pool.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKCommandPool : public CommandPool
		{
		public:
			VKCommandPool(CommandPoolDescriptor* _commandPoolDescriptor);
			virtual ~VKCommandPool();

			virtual void Reset() const override;

		private:
			friend class VKGraphicsContext; // TODO:: Remove me
			friend class VKCommandBuffer;
			VkCommandPool m_CommandPoolObj;
		};
	}
}