#include "Shader.h"

#include <iostream>
#include <fstream>

namespace vkEngine
{
	std::vector<char> vkEngine::ReadFile(std::string filename, bool debug)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (debug && !file.is_open())
			std::cout << "Failed to open file: \"" << filename << "\"" << std::endl;

		size_t filesize { static_cast<size_t>(file.tellg()) };
		std::vector<char> buffer(filesize);
		file.seekg(0);
		file.read(buffer.data(), filesize);

		file.close();
		return buffer;
	}

	vk::ShaderModule vkEngine::CreateModule(std::string filename, vk::Device device, bool debug)
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
		catch (vk::SystemError err)
		{
			if (debug) std::cout << "Failed to create shader module: \"" << filename << "\"" << std::endl;
			std::cout << err.what() << std::endl;
		}
		return nullptr;
	}
}