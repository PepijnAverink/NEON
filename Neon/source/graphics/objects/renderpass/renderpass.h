#pragma once
#include "./graphics/objects/renderpass/renderpass_descriptor.h"
#include "./graphics/objects/framebuffer/framebuffer.h"

namespace Neon
{
	namespace Graphics
	{
		class Renderpass
		{
		public:
			static Renderpass* Create(const RenderpassDescriptor* _renderpassDescriptor);
			virtual ~Renderpass() {}

		protected:
			Renderpass(const RenderpassDescriptor* _renderpassDescriptor);

			friend class CommandBuffer;
		};
	}
}