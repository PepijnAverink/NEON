#pragma once
#include "./graphics/graphics_context.h"

namespace Neon
{
	namespace Graphics
	{
		class GLGraphicsContext : public GraphicsContext
		{
		public:
			GLGraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor);
			virtual ~GLGraphicsContext();

			virtual bool Initialize() override;
			virtual bool Terminate()  override;
		};
	}
}