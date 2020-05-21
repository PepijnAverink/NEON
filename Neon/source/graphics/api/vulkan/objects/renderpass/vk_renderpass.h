#pragma once
#include "./graphics/objects/renderpass/renderpass.h"

#include <vulkan/vulkan.h>

namespace Neon
{
	namespace Graphics
	{
		class VKRenderpass : public Renderpass
		{
		public:
			VKRenderpass(const RenderpassDescriptor* _renderpassDescriptor);
			virtual ~VKRenderpass();

		private:
			VkRenderPass	 m_RenderPassObj;
		};
	}
}