#include "Synchronisation.h"

#include <iostream>

namespace vkEngine
{
	vk::Semaphore vkEngine::CreateSemaphore(vk::Device device, bool debug)
	{
		vk::SemaphoreCreateInfo createInfo{};
		createInfo.flags = vk::SemaphoreCreateFlags();
		try
		{
			vk::Semaphore semaphore = device.createSemaphore(createInfo);
			if (debug) std::cout << "Semaphore created" << std::endl;
			return semaphore;
		}
		catch (vk::SystemError err)
		{
			if (debug) std::cout << "Semaphore creation failed\n" << err.what() << std::endl;
			return nullptr;
		}
	}

	vk::Fence vkEngine::CreateFence(vk::Device device, bool debug)
	{
		vk::FenceCreateInfo createInfo{};
		createInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;
		try
		{
			vk::Fence fence = device.createFence(createInfo);
			if (debug) std::cout << "Fence created" << std::endl;
			return fence;
		}
		catch (vk::SystemError err)
		{
			if (debug) std::cout << "Fence creation failed\n" << err.what() << std::endl;
			return nullptr;
		}
	}
}