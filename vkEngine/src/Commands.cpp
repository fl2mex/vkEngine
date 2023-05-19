#include "Commands.h"
#include "Device.h"

#include <iostream>

namespace vkEngine
{
	vk::CommandPool vkEngine::CreateCommandPool(const vk::Device device, const vk::PhysicalDevice physicalDevice,
	                                            const vk::SurfaceKHR surface, const bool debug)
	{
		const QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice, surface, debug);
		vk::CommandPoolCreateInfo createInfo{};
		createInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
		createInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
		try
		{
			return device.createCommandPool(createInfo);
		}
		catch (vk::SystemError& err)
		{
			if (debug) std::cout << "Command pool creation failed\n" << err.what() << std::endl;
			return nullptr;
		}
	}

	vk::CommandBuffer vkEngine::CreateCommandBuffers(const CommandBufferInputChunk inputChunk, const bool debug)
	{
		vk::CommandBufferAllocateInfo allocInfo{};
		allocInfo.commandPool = inputChunk.commandPool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;
		allocInfo.commandBufferCount = 1;

		for (int i = 0; i < inputChunk.swapchainFrames.size(); i++)
		{
			try
			{
				inputChunk.swapchainFrames[i].commandBuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
				if (debug) std::cout << "Command buffer allocated for frame " << i << std::endl;
			}
			catch (vk::SystemError& err)
			{
				if (debug)
					std::cout << "Command buffer allocation failed for frame " << i << "\n" << err.what() <<
						std::endl;
			}
		}

		try
		{
			vk::CommandBuffer commandBuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
			if (debug) std::cout << "Main command buffer allocated" << std::endl;
			return commandBuffer;
		}
		catch (vk::SystemError& err)
		{
			if (debug) std::cout << "Command buffer allocation failed" << std::endl;
			return nullptr;
		}
	}
}
