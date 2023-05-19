#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

namespace vkEngine
{
	bool IsSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug);
	vk::Instance CreateInstance(bool debug, const char* applicationName);
}
