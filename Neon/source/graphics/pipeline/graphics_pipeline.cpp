#include "./graphics/pipeline/graphics_pipeline.h"
#include "./graphics/graphics_driver.h"

#if defined (NEON_SUPPORT_DIRECTX11)	
#include "./graphics/api/directx11/pipeline/dx11_graphics_pipeline.h"
#endif

#if defined (NEON_SUPPORT_DIRECTX12)	
#include "./graphics/api/directx12/pipeline/dx12_graphics_pipeline.h"
#endif

#if defined(NEON_SUPPORT_VULKAN)
#include "./graphics/api/vulkan/pipeline/vk_graphics_pipeline.h"
#endif

namespace Neon
{
	namespace Graphics
	{
		GraphicsPipeline* GraphicsPipeline::Create(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
		{
			GraphicsAPI api = GraphicsDriver::GetGraphicsAPI();

			//	// Vulkan
			if (api == GraphicsAPI::VULKAN)
				return new VKGraphicsPipeline(_graphicsPipelineDescriptor);

			// DirectX11
			if (api == GraphicsAPI::DIRECTX11)
				return new DX11GraphicsPipeline(_graphicsPipelineDescriptor);

			// DirectX12
			if (api == GraphicsAPI::DIRECTX12)
				return new DX12GraphicsPipeline(_graphicsPipelineDescriptor);

			printf("[ERROR] GraphicsPipeline::Create() No valid api abstraction for found for object: GraphicsPipeline");
			return nullptr;
		}

		GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: m_ImageWidth(_graphicsPipelineDescriptor->ImageWidth)
			, m_ImageHeight(_graphicsPipelineDescriptor->ImageHeight)
			, m_RasterizerState(_graphicsPipelineDescriptor->RasterizerStateDescriptor)
		{ }
	}
}