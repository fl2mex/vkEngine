#include "DebugMessenger.h"

#include <iostream>

namespace vkEngine
{
	// More Debug Callback Boilerplate
	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "Validation layer: " << pCallbackData->pMessage << "\n";
		return VK_FALSE;
	}

	vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance& instance,
		vk::DispatchLoaderDynamic& dldi)
	{
		// The MOTHERLOAD of boilerplate
		vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(vk::DebugUtilsMessengerCreateFlagsEXT(),
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
			debugCallback, nullptr);
		// All it needs to do...
		return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
	}
}