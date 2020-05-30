#include "./graphics/api/directx11/objects/command/dx11_command_queue.h"
#include "./graphics/api/directx11/objects/command/dx11_command_buffer.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX11CommandQueue::DX11CommandQueue(CommandQueueDescriptor* _commandQueueDescriptor)
			: CommandQueue(_commandQueueDescriptor)
		{ }

		DX11CommandQueue::~DX11CommandQueue()
		{

		}

		// TODO:: implement fence ussage
		void DX11CommandQueue::ExecuteCommandBuffer(CommandBuffer* _commandBuffer, Fence* _signalFence) const
		{
			// TODO:: Restoring context is slow, do this manually
			DX11GraphicsContext::GetInstance()->GetGraphicsDeviceContext()->ExecuteCommandList(NEON_CAST(DX11CommandBuffer*, _commandBuffer)->m_DeferedListObj, TRUE);
		}
	}
}