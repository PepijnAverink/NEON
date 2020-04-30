#include "./graphics/objects/command_generic/topology_state.h"

#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/objects/command_generic/dx12_topology_state.h"
#endif

//#if defined(NEON_SUPPORT_VULKAN)
//#include "./graphics/api/vulkan/objects/command/vk_command_buffer.h"
//#endif

namespace Neon
{
	namespace Graphics
	{
		TopologyState* TopologyState::Create(const Topology _topology)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			//	// Vulkan
		//	if (api == GraphicsAPI::VULKAN)
		//		return new VKCommandBuffer(_commandBufferDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12TopologyState(_topology);

			printf("[ERROR] TopologyState::Create() No valid api abstraction for found for object: TopologyState");
			return nullptr;
		}

		TopologyState::TopologyState(const Topology _topology)
			: m_Topology(_topology)
		{ }
	}
}