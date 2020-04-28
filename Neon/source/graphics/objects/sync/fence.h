#pragma once
#include "./graphics/objects/sync/fence_descriptor.h"
#include "./graphics/objects/sync/fence_status.h"
#include "./graphics/objects/sync/fence_return.h"

namespace Neon
{
	namespace Graphics
	{
		class Fence
		{
		public:
			static Fence* Create(const FenceDescriptor* _fenceDescriptor);
			virtual ~Fence() {}

			virtual FenceReturn         WaitForFence() = 0;
			virtual const FenceStatus GetFenceStatus() = 0;
			
			virtual void Reset() const = 0;

		protected:
			Fence(const FenceDescriptor* _fenceDescriptor);

			uint64_t    m_TimeOut;
			FenceStatus m_Status;
		};
	}
}