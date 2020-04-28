#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#include "./graphics/api/vulkan/objects/command/vk_command_buffer_type.h"
#include "./graphics/api/vulkan/vk_graphics_context.h"

#include "./graphics/api/vulkan/vk_error.h"

namespace Neon
{
	namespace Graphics
	{
		// TODO:: Abstract queufamily to type
		VKCommandPool::VKCommandPool(CommandPoolDescriptor* _commandPoolDescriptor)
			: CommandPool(_commandPoolDescriptor)
		{
			// Setup the creation info
			VkCommandPoolCreateInfo poolCreateInfo = {};
			poolCreateInfo.sType			= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolCreateInfo.queueFamilyIndex = GetFamilyQueueIndexFromType(m_Type);

			// Create the commandPool object
			VK_ThrowIfFailed(vkCreateCommandPool(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &poolCreateInfo, nullptr, &m_CommandPoolObj));
		}

		VKCommandPool::~VKCommandPool()
		{
			vkDestroyCommandPool(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_CommandPoolObj, nullptr);
		}

		void VKCommandPool::Reset() const
		{
			vkResetCommandPool(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), m_CommandPoolObj, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
		}
	}
}