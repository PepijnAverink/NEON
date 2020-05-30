#include "./graphics/objects/command/command_buffer.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/objects/command/dx11_command_buffer.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		CommandBuffer* CommandBuffer::Create(const CommandBufferDescriptor* _commandBufferDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKCommandBuffer(_commandBufferDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11CommandBuffer(_commandBufferDescriptor);


			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12CommandBuffer(_commandBufferDescriptor);

			printf("[ERROR] CommandAllocator::Create() No valid api abstraction for found for object: CommandBuffer");
			return nullptr;
		}

		inline const std::string CommandBuffer::GetCommandBufferName() const
		{
#if defined(NEON_DEBUG)
			return m_Name;
#else
			return "";
#endif
		}

		CommandBuffer::CommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor)
			: m_Type(_commandBufferDescriptor->Type)
			, m_CommandPool(_commandBufferDescriptor->CommandPool)
#if defined(NEON_DEBUG)
			, m_Name(_commandBufferDescriptor->Name)
#endif
		{  }
	}
}