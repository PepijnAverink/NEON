#pragma once
#include "./graphics/objects/command/command_queue_type.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"

namespace Neon
{
	namespace Graphics
	{

		// ------------------------------------------
		// GetFamilyQueueIndexFromType function
		// a Vulkan specific function that retrieves the familyQueueType based on the general CommandQueueType enum.
		// Retrieval is done by querying the GraphicsContext implementation.
		// ------------------------------------------

		// TODO:: Support more commandQueueTypes
		inline uint32_t GetFamilyQueueIndexFromType(const CommandQueueType _type)
		{
			switch (_type)
			{
				case CommandQueueType::NEON_COMMAND_QUEUE_TYPE_DIRECT:   return VKGraphicsContext::GetInstance()->GetQueueFamilyIDGraphics();
			}

			printf("[ERROR] GetFamilyQueueIndexFromType() - This type is currently not supported by the vulkan implementation.");
			return 0;
		}
	}
}