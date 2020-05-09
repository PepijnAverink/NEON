#pragma once
#include "./graphics/objects/command/command_buffer.h"

#include <./vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKCommandBuffer final : public CommandBuffer
		{
		public:
			VKCommandBuffer(const CommandBufferDescriptor* _commandBufferDescriptor);
			virtual ~VKCommandBuffer() { }

			virtual void StartRecording()  const override;
			virtual void EndRecording()	   const override;

			virtual void Reset() const override;

			// Commands
			virtual void SetGraphicsPipeline(GraphicsPipeline* _graphicsPipeline) const override;

			virtual void SetVertexBuffer(VertexBuffer* _vertexBuffer) const override;
			virtual void SetIndexBuffer(IndexBuffer* _indexBuffer) const override;

			virtual void SetTopology(const Topology _topology) const override;
			virtual void SetTopology(TopologyState* _topologyState) const override;

			virtual void SetViewport(Viewport* _viewport) const override;
			virtual void SetScissor(Scissor* _scissor) const override;

			virtual void BeginRenderpass(Renderpass* _renderpass, Framebuffer* _framebuffer) const override;
			virtual void EndRenderpass(Renderpass* _renderpass) const override;

		private:
			friend class VKGraphicsContext; // TODO:: Remove me
			friend class VKCommandQueue;
			VkCommandBuffer			 m_CommandBufferObj;
			VkCommandBufferBeginInfo m_BeginInfo;
		};
	}
}