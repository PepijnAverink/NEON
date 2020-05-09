#pragma once
#include "./graphics/objects/renderpass/renderpass.h"

namespace Neon
{
	namespace Graphics
	{
		class DX12Renderpass : public Renderpass
		{
		public:
			DX12Renderpass(const RenderpassDescriptor* _renderpassDescriptor);
			virtual ~DX12Renderpass();

		private:

		};
	}
}