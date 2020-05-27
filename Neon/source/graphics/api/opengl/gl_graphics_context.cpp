#include "./graphics/api/opengl/gl_graphics_context.h"

namespace Neon
{
	namespace Graphics
	{
		GLGraphicsContext::GLGraphicsContext(Core::Window* _window)
			: GraphicsContext(_window)
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

		void GLGraphicsContext::Present()
		{

		}
	}
}