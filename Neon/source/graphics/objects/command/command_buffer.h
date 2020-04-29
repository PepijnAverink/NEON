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
		class CommandBuffer
		{
		public:
			static CommandBuffer* Create(const CommandBufferDescriptor* _commandBufferDescriptor);
			virtual ~CommandBuffer() {}

			virtual void StartRecording()  const = 0;
			virtual void EndRecording()	   const = 0;

			virtual void Reset() const = 0;

			// Commands
			virtual void SetViewport(const Viewport _viewport) const = 0;;

			virtual void SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) const = 0;

			virtual void SetVertexBuffer(VertexBuffer* _vertexBuffer) const = 0;
			virtual void SetIndexBuffer(IndexBuffer* _indexBuffer) const = 0;

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
	}
}