#include "./graphics/api/vulkan/objects/renderpass/vk_renderpass.h"

namespace Neon
{
	namespace Graphics
	{
		VKRenderpass::VKRenderpass(const RenderpassDescriptor* _renderpassDescriptor)
			: Renderpass(_renderpassDescriptor)
		{

		}

		VKRenderpass::~VKRenderpass()
		{

		}
	}
}