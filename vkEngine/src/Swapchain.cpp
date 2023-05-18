#include "Swapchain.h"
#include "Logging.h"
#include "Device.h"

#include <iostream>
#include <string>

namespace vkEngine
{
	SwapchainSupportDetails QuerySwapSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug)
	{
		SwapchainSupportDetails support;
		support.capabilities = device.getSurfaceCapabilitiesKHR(surface);
		support.formats = device.getSurfaceFormatsKHR(surface);
		support.presentModes = device.getSurfacePresentModesKHR(surface);
		if (debug)
		{
			std::cout << "\nSurface Capabilities:\n";
			std::cout << "\tMin Image Count: " << support.capabilities.minImageCount << "\n";
			std::cout << "\tMax Image Count: " << support.capabilities.maxImageCount << "\n";
			std::cout << "\tCurrent Extent: " << support.capabilities.currentExtent.width << "x" <<
				support.capabilities.currentExtent.height << "\n";
			std::cout << "\tMin Image Extent: " << support.capabilities.minImageExtent.width << "x" <<
				support.capabilities.minImageExtent.height << "\n";
			std::cout << "\tMax Image Extent: " << support.capabilities.maxImageExtent.width << "x" <<
				support.capabilities.maxImageExtent.height << "\n";
			std::cout << "\tMax Image Array Layers: " << support.capabilities.maxImageArrayLayers << "\n";
			std::vector<std::string> stringList{};
			stringList = LogTransformBits(support.capabilities.supportedTransforms);
			std::cout << "\tSupported Transforms:\n";
			for (std::string str : stringList)
				std::cout << "\t\t" << str << "\n";
			std::cout << "\tCurrent Transform:\n";
			stringList = LogTransformBits(support.capabilities.currentTransform);
			for (std::string str : stringList)
				std::cout << "\t\t" << str << "\n";
			std::cout << "\tSupported Composite Alpha:\n";
			stringList = LogAlphaCompositeBits(support.capabilities.supportedCompositeAlpha);
			for (std::string str : stringList)
				std::cout << "\t\t" << str << "\n";
			std::cout << "\tSupported Usage Flags:\n";
			stringList = LogImageUsageBits(support.capabilities.supportedUsageFlags);
			for (std::string str : stringList)
				std::cout << "\t\t" << str << "\n";
			std::cout << "\n";

			std::cout << "Surface Formats:\n";
			for (vk::SurfaceFormatKHR format : support.formats)
			{
				std::cout << "\tSupported Pixel Format: " << vk::to_string(format.format) << "\n";
				std::cout << "\tSupported Color Space: " << vk::to_string(format.colorSpace) << "\n";
			}
			std::cout << "\n";

			std::cout << "Surface Present Modes:\n";
			for (vk::PresentModeKHR mode : support.presentModes)
				std::cout << "\t" << LogPresentMode(mode) << "\n";
			std::cout << "\n";
		}
		return support;
	}

	vk::SurfaceFormatKHR vkEngine::ChooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> formats)
	{
		for (vk::SurfaceFormatKHR format : formats)
		{
			if (format.format == vk::Format::eB8G8R8A8Unorm &&
				format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return format;
		}
		return formats[0];
	}

	vk::PresentModeKHR vkEngine::ChooseSwapPresentMode(std::vector<vk::PresentModeKHR> modes)
	{
		for (vk::PresentModeKHR mode : modes)
		{
			if (mode == vk::PresentModeKHR::eMailbox)
				return mode;
			if (mode == vk::PresentModeKHR::eImmediate)
				return mode;
		}
		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D vkEngine::ChooseSwapExtent(uint32_t width, uint32_t height,
		vk::SurfaceCapabilitiesKHR capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		else
		{
			vk::Extent2D extent = { width, height };
			extent.width = std::min(capabilities.maxImageExtent.width,
				std::max(capabilities.minImageExtent.width, width));
			extent.height = std::min(capabilities.maxImageExtent.height,
				std::max(capabilities.minImageExtent.height, height));
			return extent;
		}
	}

	SwapchainBundle CreateSwapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice,
		vk::SurfaceKHR surface, uint32_t width, uint32_t height, bool debug)
	{
		SwapchainSupportDetails support = QuerySwapSupport(physicalDevice, surface, debug);
		vk::SurfaceFormatKHR format = ChooseSwapSurfaceFormat(support.formats);
		vk::PresentModeKHR mode = ChooseSwapPresentMode(support.presentModes);
		vk::Extent2D extent = ChooseSwapExtent(width, height, support.capabilities);
		uint32_t imageCount = std::min(support.capabilities.maxImageCount,
			support.capabilities.minImageCount + 1);

		vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(),
			surface, imageCount, format.format, format.colorSpace, extent, 1,
			vk::ImageUsageFlagBits::eColorAttachment);

		QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface, debug);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily.value() != indices.presentFamily.value())
		{
			createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else createInfo.imageSharingMode = vk::SharingMode::eExclusive;
		createInfo.preTransform = support.capabilities.currentTransform;
		createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
		createInfo.presentMode = mode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

		SwapchainBundle bundle{};
		try
		{
			bundle.swapchain = logicalDevice.createSwapchainKHR(createInfo);
		}
		catch (vk::SystemError err)
		{
			throw std::runtime_error("Failed to create swap chain!");
		}

		std::vector <vk::Image> images = logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
		bundle.frames.resize(images.size());
		for (size_t i = 0; i < images.size(); i++)
		{
			vk::ImageViewCreateInfo createInfo = vk::ImageViewCreateInfo(vk::ImageViewCreateFlags(),
				images[i], vk::ImageViewType::e2D, format.format, { vk::ComponentSwizzle::eIdentity,
				vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity},
				vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

			bundle.frames[i].image = images[i];
			bundle.frames[i].imageView = logicalDevice.createImageView(createInfo);
		}
		bundle.format = format.format;
		bundle.extent = extent;
		return bundle;
	}
}