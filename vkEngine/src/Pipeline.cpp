#include "Pipeline.h"

#include <iostream>

namespace vkEngine
{
	GraphicsPipelineOut vkEngine::CreateGraphicsPipeline(GraphicsPipelineIn specs, const bool debug)
	{
		vk::GraphicsPipelineCreateInfo createInfo{};
		createInfo.flags = vk::PipelineCreateFlags();

		std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

		// Vertex input
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.flags = vk::PipelineVertexInputStateCreateFlags();
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		createInfo.pVertexInputState = &vertexInputInfo;

		// Input assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.flags = vk::PipelineInputAssemblyStateCreateFlags();
		inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
		createInfo.pInputAssemblyState = &inputAssembly;

		// Vertex shader
		if (debug) std::cout << "Creating vertex shader module" << std::endl;
		vk::ShaderModule vertexShader = CreateModule(specs.vertexFilePath, specs.device, debug);
		vk::PipelineShaderStageCreateInfo vertexShaderInfo{};
		vertexShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		vertexShaderInfo.stage = vk::ShaderStageFlagBits::eVertex;
		vertexShaderInfo.module = vertexShader;
		vertexShaderInfo.pName = "main";
		shaderStages.push_back(vertexShaderInfo);

		// Viewport
		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = specs.swapchainExtent.width;
		viewport.height = specs.swapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor
		vk::Rect2D scissor{};
		scissor.offset.x = 0.0f;
		scissor.offset.y = 0.0f;
		scissor.extent = specs.swapchainExtent;

		vk::PipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.flags = vk::PipelineViewportStateCreateFlags();
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = &viewport;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = &scissor;
		createInfo.pViewportState = &viewportInfo;

		// Rasterizer
		vk::PipelineRasterizationStateCreateInfo rasterizerInfo{};
		rasterizerInfo.flags = vk::PipelineRasterizationStateCreateFlags();
		rasterizerInfo.depthClampEnable = VK_FALSE;
		rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizerInfo.polygonMode = vk::PolygonMode::eFill;
		rasterizerInfo.lineWidth = 1.0f;
		rasterizerInfo.cullMode = vk::CullModeFlagBits::eBack;
		rasterizerInfo.frontFace = vk::FrontFace::eClockwise;
		rasterizerInfo.depthBiasEnable = VK_FALSE;
		createInfo.pRasterizationState = &rasterizerInfo;

		// Fragment shader
		if (debug) std::cout << "Creating fragment shader module" << std::endl;
		vk::ShaderModule fragmentShader = CreateModule(specs.fragmentFilePath, specs.device, debug);
		vk::PipelineShaderStageCreateInfo fragmentShaderInfo{};
		fragmentShaderInfo.flags = vk::PipelineShaderStageCreateFlags();
		fragmentShaderInfo.stage = vk::ShaderStageFlagBits::eFragment;
		fragmentShaderInfo.module = fragmentShader;
		fragmentShaderInfo.pName = "main";
		shaderStages.push_back(fragmentShaderInfo);

		createInfo.stageCount = shaderStages.size();
		createInfo.pStages = shaderStages.data();

		// Multisampling
		vk::PipelineMultisampleStateCreateInfo multisamplingInfo{};
		multisamplingInfo.flags = vk::PipelineMultisampleStateCreateFlags();
		multisamplingInfo.sampleShadingEnable = VK_FALSE;
		multisamplingInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
		createInfo.pMultisampleState = &multisamplingInfo;

		// Color blending
		vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
		colorBlendAttachment.blendEnable = VK_FALSE;

		vk::PipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.flags = vk::PipelineColorBlendStateCreateFlags();
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = vk::LogicOp::eCopy;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		for (int i = 0; i < 4; i++)
			colorBlending.blendConstants[i] = 0.0f;
		createInfo.pColorBlendState = &colorBlending;

		if (debug) std::cout << "Creating pipeline layout" << std::endl;
		vk::PipelineLayout layout = CreatePipelineLayout(specs.device, debug);
		createInfo.layout = layout;

		if (debug) std::cout << "Creating render pass" << std::endl;
		vk::RenderPass renderPass = CreateRenderPass(specs.device, specs.swapchainImageFormat, debug);
		createInfo.renderPass = renderPass;

		createInfo.basePipelineHandle = nullptr;

		if (debug) std::cout << "Creating graphics pipeline" << std::endl;

		vk::Pipeline graphicsPipeline;
		try
		{
			graphicsPipeline = specs.device.createGraphicsPipeline(nullptr, createInfo).value;
		}
		catch (vk::SystemError& err)
		{
			if (debug) std::cout << "Failed to create graphics pipeline\n" << err.what() << std::endl;
		}

		GraphicsPipelineOut out{};

		out.layout = layout;
		out.renderPass = renderPass;
		out.pipeline = graphicsPipeline;

		specs.device.destroyShaderModule(vertexShader);
		specs.device.destroyShaderModule(fragmentShader);
		return out;
	}

	vk::PipelineLayout vkEngine::CreatePipelineLayout(const vk::Device device, const bool debug)
	{
		vk::PipelineLayoutCreateInfo createInfo{};
		createInfo.flags = vk::PipelineLayoutCreateFlags();
		createInfo.setLayoutCount = 0;
		createInfo.pushConstantRangeCount = 0;
		try
		{
			return device.createPipelineLayout(createInfo);
		}
		catch (vk::SystemError& err)
		{
			if (debug) std::cout << "Failed to create pipeline layout\n" << err.what() << std::endl;
		}
	}

	vk::RenderPass vkEngine::CreateRenderPass(const vk::Device device, const vk::Format swapchainImageFormat,
	                                          const bool debug)
	{
		vk::AttachmentDescription colorAttachment{};
		colorAttachment.format = swapchainImageFormat;
		colorAttachment.samples = vk::SampleCountFlagBits::e1;
		colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
		colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
		colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
		colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
		colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

		vk::AttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

		vk::SubpassDescription subpass{};
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		vk::RenderPassCreateInfo createInfo{};
		createInfo.flags = vk::RenderPassCreateFlags();
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colorAttachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		try
		{
			return device.createRenderPass(createInfo);
		}
		catch (vk::SystemError& err)
		{
			if (debug) std::cout << "Failed to create render pass\n" << err.what() << std::endl;
		}
	}
}
