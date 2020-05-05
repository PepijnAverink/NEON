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

			virtual void BeginPass(Framebuffer* _framebuffer) const = 0;
			virtual void EndPass() const = 0;

		protected:
			Renderpass(const RenderpassDescriptor* _renderpassDescriptor);
		};
	}
}