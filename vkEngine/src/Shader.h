#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>

namespace vkEngine
{
	std::vector<char> ReadFile(std::string filename, bool debug);
	vk::ShaderModule CreateModule(std::string filename, vk::Device device, bool debug);
}
