#include "./graphics/objects/command/command_queue.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/command/dx12_command_queue.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/command/vk_command_queue.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		CommandQueue* CommandQueue::Create(CommandQueueDescriptor* _commandQueueDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKCommandQueue(_commandQueueDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12CommandQueue(_commandQueueDescriptor);

			printf("[ERROR] CommandAllocator::Create() No valid api abstraction for found for object: CommandQueue");
			return nullptr;
		}

		inline const std::string CommandQueue::GetCommandQueueName() const
		{
#if defined(NEON_DEBUG)
			return m_Name;
#else
			return "";
#endif
		}
		CommandQueue::CommandQueue(CommandQueueDescriptor* _commandQueueDescriptor)
			: m_QueueIndex(_commandQueueDescriptor->QueueIndex)
			, m_Type(_commandQueueDescriptor->Type)
#if defined(NEON_DEBUG)
			, m_Name(_commandQueueDescriptor->Name)
#endif
		{ }
	}
}