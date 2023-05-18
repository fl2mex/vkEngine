#include "Device.h"
#include "Logging.h"

#include <iostream>
#include <string>
#include <set>

namespace vkEngine
{
	bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device,
		const std::vector<const char*>& requestedExtensions, const bool& debug)
	{
		std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());
		if (debug) std::cout << "Device can support extensions:\n";

		for (vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties())
		{
			if (debug) std::cout << "\t\"" << extension.extensionName << "\"\n";
			requiredExtensions.erase(extension.extensionName);
		}
		std::cout << "\n";
		return requiredExtensions.empty();
	}

	bool isSuitable(const vk::PhysicalDevice& device, const bool debug) {

		const std::vector<const char*> requestedExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		if (debug)
		{
			std::cout << "Checking if device is suitable\nRequesting device extensions:\n";
			for (const char* extension : requestedExtensions)
				std::cout << "\t\"" << extension << "\"\n";
			std::cout << "\n";
		}

		if (bool extensionsSupported = checkDeviceExtensionSupport(device, requestedExtensions, debug))
			if (debug) std::cout << "Device can support the requested extensions\n\n";
			else
			{
				if (debug) std::cout << "Device can't support the requested extensions\n\n";
				return false;
			}
		return true;
	}

	vk::PhysicalDevice CreateDevice(const vk::Instance& instance, const bool debug)
	{
		std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
		if (debug) std::cout << "Choosing Physical Device\nThere are " <<
			availableDevices.size() << " physical devices available on this system\n";

		for (vk::PhysicalDevice device : availableDevices)
		{
			if (debug) LogDeviceProperties(device);
			if (isSuitable(device, debug)) return device;
		}
		return nullptr;
	}

	QueueFamilyIndices vkEngine::FindQueueFamilies(vk::PhysicalDevice device,
		vk::SurfaceKHR surface, bool debug)
	{
		QueueFamilyIndices indices;
		// TODO: Figure out why this is not working.
		// device.getQueueFamilyProperties() does not run when debug is turned off. Must figure out.
		std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

		if (debug) std::cout << "There are " << queueFamilies.size() <<
			" queue families available on the system.\n";

		int i = 0;
		for (vk::QueueFamilyProperties queueFamily : queueFamilies)
		{
			if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				indices.graphicsFamily = i;
				if (debug) std::cout << "Queue Family " << i << " is suitable for graphics\n";
			}

			if (device.getSurfaceSupportKHR(i, surface))
			{
				indices.presentFamily = i;
				if (debug) std::cout << "Queue Family " << i << " is suitable for presenting\n";
			}

			if (indices.isComplete()) break;
			i++;
		}
		std::cout << "\n";
		return indices;
	}

	vk::Device vkEngine::CreateLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface,
		bool debug)
	{
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface, debug);
		std::vector<uint32_t> uniqueIndices;
		uniqueIndices.push_back(indices.graphicsFamily.value());
		if (indices.presentFamily.value() != indices.graphicsFamily.value())
			uniqueIndices.push_back(indices.presentFamily.value());

		float queuePriority = 1.0f;
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
		for (uint32_t queueFamilyIndex : uniqueIndices)
		{
			queueCreateInfo.push_back(vk::DeviceQueueCreateInfo(vk::DeviceQueueCreateFlags(),
				queueFamilyIndex, 1, &queuePriority));
		}

		std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
		std::vector<const char*> enabledLayers;
		if (debug) enabledLayers.push_back("VK_LAYER_KHRONOS_validation");

		vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo(vk::DeviceCreateFlags(), 
			queueCreateInfo.size(), queueCreateInfo.data(),
			enabledLayers.size(), enabledLayers.data(),
			deviceExtensions.size(), deviceExtensions.data(), &deviceFeatures);

		try
		{
			vk::Device device = physicalDevice.createDevice(createInfo);
			if (debug) std::cout << "GPU successfully created\n";
			return device;
		}
		catch (vk::SystemError err)
		{
			if (debug)
			{
				std::cout << "GPU creation failed\n" << err.what() << "\n";
			}
			return nullptr;
		}
		return nullptr;
	}

	 std::array<vk::Queue, 2> vkEngine::GetQueue(vk::PhysicalDevice physicalDevice, vk::Device device,
		vk::SurfaceKHR surface, bool debug)
	 {
		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface, debug);

		return
		{
			device.getQueue(indices.graphicsFamily.value(), 0),
			device.getQueue(indices.presentFamily.value(), 0)
		};
	}
}