#pragma once
#include "./graphics/objects/command/command_buffer_type.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"

namespace Neon
{
	namespace Graphics
	{

		// ------------------------------------------
		// GetFamilyQueueIndexFromType function
		// a Vulkan specific function that retrieves the familyQueueType based on the general CommandBufferType enum.
		// Retrieval is done by querying the GraphicsContext implementation.
		// ------------------------------------------
		inline uint32_t GetFamilyQueueIndexFromType(const CommandBufferType _type)
		{
			switch (_type)
			{
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT:   return VKGraphicsContext::GetInstance()->GetQueueFamilyIDGraphics();
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_INDIRECT: return VKGraphicsContext::GetInstance()->GetQueueFamilyIDGraphics();
			}

			printf("[ERROR] GetFamilyQueueIndexFromType() - This type is currently not supported by the vulkan implementation.");
			return 0;
		}

		inline VkCommandBufferLevel GetCommandBufferLevelFromType(const CommandBufferType _type)
		{
			switch (_type)
			{
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_DIRECT:   return VK_COMMAND_BUFFER_LEVEL_PRIMARY;
				case CommandBufferType::NEON_COMMAND_BUFFER_TYPE_INDIRECT: return VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			}

			printf("[WARNING] GetCommandBufferLevelFromType() - No clear level was specified, this doesnt need to be an issue default to Direct.");
			return VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		}
	}
}