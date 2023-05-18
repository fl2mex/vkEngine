#pragma once
#include "Swapchain.h"

#include <vulkan/vulkan.hpp>

namespace vkEngine
{
	struct FrameBufferInput
	{
		vk::Device device;
		vk::RenderPass renderPass;
		vk::Extent2D swapchainExtent;
	};

	void CreateFrameBuffers(FrameBufferInput input, std::vector<vkEngine::SwapchainFrame>& frames, bool debug);
}