#pragma once
#include <vulkan/vulkan.hpp>

namespace vkEngine
{
	vk::Semaphore CreateSemaphore(vk::Device device, bool debug);
	vk::Fence CreateFence(vk::Device device, bool debug);
}
