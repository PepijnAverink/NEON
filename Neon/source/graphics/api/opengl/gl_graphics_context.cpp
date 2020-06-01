#include "./graphics/api/opengl/gl_graphics_context.h"

namespace Neon
{
	namespace Graphics
	{
		GLGraphicsContext::GLGraphicsContext(const GraphicsContextDescriptor* _graphicsContextDescriptor)
			: GraphicsContext(_graphicsContextDescriptor)
		{

		}

		GLGraphicsContext::~GLGraphicsContext()
		{

		}

		bool GLGraphicsContext::Initialize()
		{
			return false;
		}

		bool GLGraphicsContext::Terminate()
		{
			return false;
		}
	}
}