#pragma once
#include "./graphics/objects/command/command_buffer_descriptor.h"
#include "./graphics/objects/command/command_buffer_state.h"

#include "./graphics/graphics_defines.h"

namespace Neon
{
	namespace Graphics
	{
		// ------------------------------------------
		// CommandBuffer class
		// Serves as an abstraction of a commandBuffer or commandList in respective API's
		// Object is initialized using CommandBufferDescriptor object, which is specified by the user.
		// The name is stored for debugging reasons and is not compiled in release mode, the getter will return an empty string in release mode.
		// ------------------------------------------
		class Viewport;
		class GraphicsPipeline;
		class VertexBuffer;
		class IndexBuffer;

		class Viewport;
		class Scissor;

		class Framebuffer;

		class FramebufferAttachment;
		enum  FramebufferAttachmentTransitionState;

		class Texture2D;

		class ComputeBuffer;
		class ComputeShader;

		class CommandBuffer
		{
		public:
			static CommandBuffer* Create(const CommandBufferDescriptor* _commandBufferDescriptor);
			virtual ~CommandBuffer();

			virtual void StartRecording() = 0;
			virtual void EndRecording()   = 0;

			virtual void Reset() = 0;

			// Commands
			virtual void SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) = 0;

			virtual void BindVertexBuffer(VertexBuffer* _vertexBuffer) const = 0;
			virtual void BindIndexBuffer(IndexBuffer* _indexBuffer) const = 0;

			virtual void BindComputeShader(ComputeShader* _computeShader) = 0;
			virtual void BindComputeBuffer(ComputeBuffer* _computeBuffer) = 0;
			virtual void DispatchCompute(const uint32_t _x, const uint32_t _y, const uint32_t _z) = 0;

			virtual void BindTexture(Texture2D* _texture, uint32_t _bindPoint) const = 0;
			virtual void BindTexture(FramebufferAttachment* _framebufferAttachment, uint32_t _bindPoint) const = 0;

			virtual void SetViewport(Viewport* _viewport) const = 0;
			virtual void SetScissor(Scissor* _scissor) const = 0;

			virtual void ClearFrameBuffer(Framebuffer* _framebuffer, const float* _color, const uint32_t _offset, const uint32_t _count, uint32_t _flags) const = 0;

			virtual void BeginRenderpass(Framebuffer* _framebuffer) const = 0;
			virtual void EndRenderpass() const = 0;

			virtual void TransitionFramebufferAttachment(FramebufferAttachment* _framebufferAttachment, const FramebufferAttachmentTransitionState _fromState, const FramebufferAttachmentTransitionState _toState) const = 0;

			virtual void DrawIndexed(const uint32_t _indexCount, const uint32_t _indexOffset, uint32_t _vertexOffset) const = 0;

			// Getters
			inline const std::string        GetCommandBufferName()  const;
			inline const CommandBufferType  GetCommandBufferType()  const { return m_Type;  }
			inline const CommandBufferState GetCommandBufferState() const { return m_State; }
			
		protected:
			CommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor);

#if defined(NEON_DEBUG)
			std::string		   m_Name;
#endif

			CommandBufferType  m_Type;
			CommandBufferState m_State = CommandBufferState::NEON_COMMAND_BUFFER_STATE_INITIAL;

			CommandPool*	   m_CommandPool = nullptr;
		};
	};
};