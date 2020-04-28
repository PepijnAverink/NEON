#pragma once
#include "./graphics/objects/sync/fence.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKFence : public Fence
		{
		public:
			VKFence(const FenceDescriptor* _fenceDestriptor);
			virtual ~VKFence();

			virtual FenceReturn         WaitForFence() override;
			virtual const FenceStatus GetFenceStatus() override;

			virtual void Reset() const override;

		private:
			friend class VKGraphicsContext; // Remove this dependency
			friend class VKCommandQueue;
			VkFence m_FenceObj;

		};
	}
}