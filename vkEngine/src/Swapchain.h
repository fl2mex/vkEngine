#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>

namespace vkEngine
{
	struct SwapchainSupportDetails
	{
		vk::SurfaceCapabilitiesKHR capabilities;
		std::vector<vk::SurfaceFormatKHR> formats;
		std::vector<vk::PresentModeKHR> presentModes;
	};
	struct SwapchainFrame
	{
		vk::Image image;
		vk::ImageView imageView;
	};
	struct SwapchainBundle
	{
		vk::SwapchainKHR swapchain;
		std::vector<SwapchainFrame> frames;
		vk::Format format;
		vk::Extent2D extent;
	};

	SwapchainSupportDetails QuerySwapSupport(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug);
	vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(std::vector<vk::SurfaceFormatKHR> formats);
	vk::PresentModeKHR ChooseSwapPresentMode(std::vector<vk::PresentModeKHR> modes);
	vk::Extent2D ChooseSwapExtent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities);
	SwapchainBundle CreateSwapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice,
		vk::SurfaceKHR surface, uint32_t width, uint32_t height, bool debug);
}