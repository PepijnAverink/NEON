#pragma once
#include "./graphics/resources/texture/texture2D_descriptor.h"
#include "./graphics/objects/command/command_buffer.h"

namespace Neon
{
	namespace Graphics
	{
		class Texture2D
		{
		public:
			static Texture2D* Load(const std::string& _filename);
			static Texture2D* Create(CommandBuffer* _commandBuffer, const Texture2DDescriptor* _texture2DDescriptor);

		protected:
			Texture2D(const Texture2DDescriptor* _texture2DDectiptor);

			unsigned int m_Width;
			unsigned int m_Height;
		};
	}
}