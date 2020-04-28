#include "./graphics/objects/command/command_pool.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/command/dx12_command_pool.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		CommandPool* CommandPool::Create(CommandPoolDescriptor* _commandPoolDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKCommandPool(_commandPoolDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12CommandPool(_commandPoolDescriptor);

			printf("[ERROR] CommandAllocator::Create() No valid api abstraction for found for object: CommandPool");
			return nullptr;
		}

		uint32_t CommandPool::AddCommandBuffer(const CommandBuffer* _commandBuffer)
		{
			m_CommandBufferCount++;
			return (m_CommandBufferCount - 1);
		}

		const std::string CommandPool::GetCommandBufferName() const
		{
#if defined(NEON_DEBUG)
			return m_Name;
#else
			return "";
#endif
		}

		CommandPool::CommandPool(CommandPoolDescriptor* _commandPoolDescriptor)
			: m_Type(_commandPoolDescriptor->Type)
#if defined(NEON_DEBUG)
			, m_Name(_commandPoolDescriptor->Name)
#endif
		{ }
	}
}