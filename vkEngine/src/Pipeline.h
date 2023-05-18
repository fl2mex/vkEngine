#pragma once
#include "Shader.h"

namespace vkEngine
{
	struct GraphicsPipelineIn
	{
		vk::Device device;
		std::string vertexFilePath;
		std::string fragmentFilePath;
		vk::Extent2D swapchainExtent;
		vk::Format swapchainImageFormat;
	};
	struct GraphicsPipelineOut
	{
		vk::PipelineLayout layout;
		vk::RenderPass renderPass;
		vk::Pipeline pipeline;
	};

	vk::PipelineLayout CreatePipelineLayout(vk::Device device, bool debug);
	GraphicsPipelineOut CreateGraphicsPipeline(GraphicsPipelineIn specs, bool debug);
	vk::RenderPass CreateRenderPass(vk::Device device, vk::Format swapchainImageFormat, bool debug);
}