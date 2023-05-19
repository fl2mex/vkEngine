#include "Instance.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

namespace vkEngine
{
	bool IsSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers,
	                 const bool debug)
	{
		const std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
		if (debug)
		{
			std::cout << "Device can support the following extensions:\n";
			for (vk::ExtensionProperties supportedExtension : supportedExtensions)
				std::cout << '\t' << supportedExtension.extensionName << "\n";
			std::cout << "\n";
		}

		bool found;
		for (const char* extension : extensions) // Loop then check whether the extension is supported
		{
			found = false;
			for (vk::ExtensionProperties supportedExtension : supportedExtensions)
			{
				if (strcmp(extension, supportedExtension.extensionName) == 0)
				{
					found = true;
					if (debug) std::cout << "Extension \"" << extension << "\" is supported!\n";
				}
			}
			if (!found)
			{
				if (debug) std::cout << "Extension \"" << extension << "\" is not supported!\n\n";
				return false;
			}
		}
		std::cout << "\n";

		const std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
		if (debug)
		{
			std::cout << "Device can support the following layers:\n";
			for (vk::LayerProperties supportedLayer : supportedLayers)
				std::cout << '\t' << supportedLayer.layerName << "\n";
			std::cout << "\n";
		}

		for (const char* layer : layers) // Loop then check whether the layer is supported
		{
			found = false;
			for (vk::LayerProperties supportedLayer : supportedLayers)
			{
				if (strcmp(layer, supportedLayer.layerName) == 0)
				{
					found = true;
					if (debug) std::cout << "Layer \"" << layer << "\" is supported!\n";
				}
			}
			if (!found)
			{
				if (debug) std::cout << "Layer \"" << layer << "\" is not supported!\n\n";
				return false;
			}
		}
		std::cout << "\n";
		return true;
	}

	vk::Instance CreateInstance(bool debug, const char* applicationName)
	{
		if (debug) std::cout << "\nCreating Vulkan Instance\n";
		uint32_t version{0};
		vkEnumerateInstanceVersion(&version);
		if (debug)
			std::cout << "Vulkan Variant: " <<
				VK_API_VERSION_VARIANT(version) << "\nVersion: " << VK_API_VERSION_MAJOR(version) <<
				"." << VK_API_VERSION_MINOR(version) << "." << VK_API_VERSION_PATCH(version) << "\n\n";

		version &= ~(0xFFFU);
		version = VK_MAKE_API_VERSION(0, 1, 0, 0);

		const vk::ApplicationInfo appInfo(applicationName, version, "No Engine", version, version);

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		std::vector<const char*> layers;

		if (debug)
		{
			extensions.push_back("VK_EXT_debug_utils"); // Needed for DebugUtilsMessengerEXT
			layers.push_back("VK_LAYER_KHRONOS_validation"); // Needed for validating extensions & layers
			std::cout << "Extensions requested:\n";
			for (const char* extensionName : extensions) // Loop then print extension names
				std::cout << "\t\"" << extensionName << "\"\n";
			std::cout << "\n";
		}

		if (!IsSupported(extensions, layers, debug)) return nullptr;

		const vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo,
		                                                                 layers.size(), layers.data(),
		                                                                 extensions.size(), extensions.data());

		try
		{
			const vk::Instance instance = vk::createInstance(createInfo);
			if (debug) std::cout << "Created Vulkan Instance\n\n";
			return instance;
		}
		catch (vk::SystemError& err)
		{
			// err.what() is the best naming scheme i've ever come across
			std::cout << "Failed to create Vulkan Instance\n" << err.what() << "\n";
			return nullptr;
		}
	}
}
