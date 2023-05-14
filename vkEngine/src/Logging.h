#pragma once
#include <vulkan/vulkan.hpp>

namespace vkEngine
{
	// Debug Callback boilerplate
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance& instance,
		vk::DispatchLoaderDynamic& dldi);

	void LogDeviceProperties(vk::PhysicalDevice& device);

	std::vector<std::string> LogTransformBits(vk::SurfaceTransformFlagsKHR bits);
	std::vector<std::string> LogAlphaCompositeBits(vk::CompositeAlphaFlagsKHR bits);
	std::vector<std::string> LogImageUsageBits(vk::ImageUsageFlags bits);

	std::string LogPresentMode(vk::PresentModeKHR mode);
}