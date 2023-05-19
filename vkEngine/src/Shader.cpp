#include "Shader.h"

#include <iostream>
#include <fstream>

namespace vkEngine
{
	std::vector<char> vkEngine::ReadFile(const std::string filename, const bool debug)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (debug && !file.is_open())
			std::cout << "Failed to open file: \"" << filename << "\"" << std::endl;

		size_t fileSize{static_cast<size_t>(file.tellg())};
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}

	vk::ShaderModule vkEngine::CreateModule(const std::string filename, const vk::Device device, const bool debug)
	{
		std::vector<char> source = ReadFile(filename, debug);
		vk::ShaderModuleCreateInfo createInfo = {};
		createInfo.flags = vk::ShaderModuleCreateFlags();
		createInfo.codeSize = source.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());
		try
		{
			return device.createShaderModule(createInfo);
		}
		catch (vk::SystemError& err)
		{
			if (debug) std::cout << "Failed to create shader module: \"" << filename << "\"" << std::endl;
			std::cout << err.what() << std::endl;
		}
		return nullptr;
	}
}
