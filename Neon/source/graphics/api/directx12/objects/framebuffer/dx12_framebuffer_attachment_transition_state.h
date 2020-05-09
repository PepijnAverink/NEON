#pragma once
#include "./graphics/objects/framebuffer/framebuffer_attachment_transition_state.h"

#include <iostream>
#include <d3d12.h>

namespace Neon
{
	namespace Graphics
	{
		D3D12_RESOURCE_STATES GetDX12FramebufferTransitionState(const FramebufferAttachmentTransitionState _framebufferTransitionState)
		{
			switch (_framebufferTransitionState)
			{
				case FramebufferAttachmentTransitionState::NEON_FRAMEBUFFER_TRANSITION_STATE_RENDER:  return D3D12_RESOURCE_STATE_RENDER_TARGET;
				case FramebufferAttachmentTransitionState::NEON_FRAMEBUFFER_TRANSITION_STATE_PRESENT: return D3D12_RESOURCE_STATE_PRESENT;
			}

			printf("[ERROR] GetDX12FramebufferTransitionState() - No valid instance was found!");
			return D3D12_RESOURCE_STATE_COMMON;
		}
	}
}