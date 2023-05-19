#pragma once
#include "Swapchain.h"

#include <vulkan/vulkan.hpp>

namespace vkEngine
{
	struct CommandBufferInputChunk
	{
		vk::Device device;
		vk::CommandPool commandPool;
		std::vector<SwapchainFrame>& swapchainFrames;
	};

	vk::CommandPool CreateCommandPool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface,
	                                  bool debug);

	vk::CommandBuffer CreateCommandBuffers(CommandBufferInputChunk inputChunk, bool debug);
}
