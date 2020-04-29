#pragma once
#include "./graphics/pipeline/inputLayout/input_layout.h"

namespace Neon
{
	namespace Graphics
	{
		class ShaderReflection
		{
		public:
			ShaderReflection() = default;

			InputLayout  Layout;
		};
	}
}