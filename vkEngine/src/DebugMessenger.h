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
}