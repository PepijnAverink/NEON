#include "./graphics/api/vulkan/pipeline/vk_graphics_pipeline.h"
//#include "./graphics/api/vulkan/pipeline/topology/vk_topology.h"

#include "./graphics/api/vulkan/pipeline/rasterizer/vk_cull_faces.h"
#include "./graphics/api/vulkan/pipeline/rasterizer/vk_cull_modes.h"
#include "./graphics/api/vulkan/pipeline/rasterizer/vk_fill_modes.h"

#include "./graphics/api/vulkan/vk_graphics_context.h"
#include "./graphics/api/vulkan/vk_error.h"

namespace Neon
{
	namespace Graphics
	{
		VKGraphicsPipeline::VKGraphicsPipeline(const GraphicsPipelineDescriptor* _graphicsPipelineDescriptor)
			: GraphicsPipeline(_graphicsPipelineDescriptor)
		{
			// ----------------------------------
			// RenderPass
			// ----------------------------------
			VkAttachmentDescription colorAttachment{};
		//	colorAttachment.format			= swapChainImageFormat;
			colorAttachment.samples			= VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef{};
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;

			VkRenderPassCreateInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = 1;
			renderPassInfo.pAttachments = &colorAttachment;
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpass;

			VK_ThrowIfFailed(vkCreateRenderPass(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &renderPassInfo, nullptr, &m_RenderPass));




	//		auto vertShaderCode = readFile("shaders/vert.spv");
	//		auto fragShaderCode = readFile("shaders/frag.spv");
	//
	//		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	//		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
	//
	//		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	//		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	//		vertShaderStageInfo.module = vertShaderModule;
	//		vertShaderStageInfo.pName = "main";
	//
	//		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	//		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	//		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	//		fragShaderStageInfo.module = fragShaderModule;
	//		fragShaderStageInfo.pName = "main";
	//
	//		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };




			VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
			vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount	= 0;
			vertexInputInfo.vertexAttributeDescriptionCount = 0;

			VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
			inputAssembly.sType								= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology							= VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			inputAssembly.primitiveRestartEnable			= VK_FALSE; // TODO:: Look at abstraction options

			// TODO:: Include min/max depth
			VkViewport viewport = { 0, 0, 1280, 720, 0, 1 };
			VkRect2D   scissor  = { 0, 0, 1280, 720 };

			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType								= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount						= 1;
			viewportState.pViewports						= &viewport;
			viewportState.scissorCount						= 1;
			viewportState.pScissors							= &scissor;

			// Rasterizer state
			VkPipelineRasterizationStateCreateInfo rasterizer = {};
			rasterizer.sType								= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable						= VK_FALSE;
			rasterizer.rasterizerDiscardEnable				= VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;// GetVKFillMode(m_RasterizerState.RasterizerFillMode);
			rasterizer.lineWidth = 1.0f;// m_RasterizerState.RasterizerLineWidth;
			rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;// GetVKCullMode(m_RasterizerState.RasterizerCullMode);
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;// GetVKCullFace(m_RasterizerState.RasterizerCullFace);
			rasterizer.depthBiasEnable						= VK_FALSE;

			VkPipelineMultisampleStateCreateInfo multisampling = {};
			multisampling.sType								= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable				= VK_FALSE;
			multisampling.rasterizationSamples				= VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask				= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable				= VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending = {};
			colorBlending.sType								= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable						= VK_FALSE;
			colorBlending.logicOp							= VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount					= 1;
			colorBlending.pAttachments						= &colorBlendAttachment;
			colorBlending.blendConstants[0]					= 0.0f;
			colorBlending.blendConstants[1]					= 0.0f;
			colorBlending.blendConstants[2]					= 0.0f;
			colorBlending.blendConstants[3]					= 0.0f;

			VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pushConstantRangeCount = 0;

			// Create pipeline layout
			VK_ThrowIfFailed(vkCreatePipelineLayout(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout));

			VkGraphicsPipelineCreateInfo pipelineInfo = {};
			pipelineInfo.sType								= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount							= 2;
		//	pipelineInfo.pStages							= shaderStages;
			pipelineInfo.pVertexInputState					= &vertexInputInfo;
			pipelineInfo.pInputAssemblyState				= &inputAssembly;
			pipelineInfo.pViewportState						= &viewportState;
			pipelineInfo.pRasterizationState				= &rasterizer;
			pipelineInfo.pMultisampleState					= &multisampling;
			pipelineInfo.pColorBlendState					= &colorBlending;
			pipelineInfo.layout								= m_PipelineLayout;
			pipelineInfo.renderPass							= m_RenderPass;
			pipelineInfo.subpass							= 0;
			pipelineInfo.basePipelineHandle					= VK_NULL_HANDLE;

			// Create graphics pipelineObj
			VK_ThrowIfFailed(vkCreateGraphicsPipelines(VKGraphicsContext::GetInstance()->GetGraphicsDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline));
		}
	}
}