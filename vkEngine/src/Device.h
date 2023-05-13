#pragma once
#include <vulkan/vulkan.hpp>
#include <vector>
#include <optional>

namespace vkEngine
{
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
	};

	bool checkDeviceExtensionSupport(const vk::PhysicalDevice& device,
		const std::vector<const char*>& requestedExtensions, const bool& debug);
	bool isSuitable(const vk::PhysicalDevice& device, const bool debug);

	vk::PhysicalDevice CreateDevice(const vk::Instance& instance, const bool debug);
	QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug);
	vk::Device CreateLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debug);

	std::array<vk::Queue, 2> GetQueue(vk::PhysicalDevice physicalDevice, vk::Device device,
		vk::SurfaceKHR surface, bool debug);
}