#pragma once
#include "./graphics/objects/command/command_queue_type.h"

namespace Neon
{
	namespace Graphics
	{
		class CommandQueueLayoutElement
		{
		public:
			CommandQueueType	Type;
			int					Count;
			bool				PresentSupport;
		};
	}
}