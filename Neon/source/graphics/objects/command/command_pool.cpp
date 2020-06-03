#include "./graphics/objects/command/command_pool.h"
#include "./graphics/objects/command/command_buffer.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/objects/command/dx11_command_pool.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/command/dx12_command_pool.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/command/vk_command_pool.h"
#endif

// Helper function
template <typename element_t>
auto iteratorOf(const std::vector<element_t> &vector, const element_t &value)
{
	return std::find(vector.begin(), vector.end(), value);
}

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
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11CommandPool(_commandPoolDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12CommandPool(_commandPoolDescriptor);

			printf("[ERROR] CommandAllocator::Create() No valid api abstraction for found for object: CommandPool");
			return nullptr;
		}

		void CommandPool::AddCommandBuffer(CommandBuffer* _commandBuffer)
		{
			m_CommandBufferCount++;
			m_CommandBuffers.push_back(_commandBuffer);
		}

		void CommandPool::RemoveCommandBuffer(CommandBuffer* _commandBuffer)
		{
			decltype(m_CommandBuffers)::const_iterator it = iteratorOf(m_CommandBuffers, _commandBuffer);
			if (it != m_CommandBuffers.end())
				m_CommandBuffers.erase(it);
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