#include "Logging.h"

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

	void LogDeviceProperties(vk::PhysicalDevice& device)
	{
		vk::PhysicalDeviceProperties properties = device.getProperties();
		std::cout << "Device name: " << properties.deviceName << "\nDevice type: ";
		switch (properties.deviceType)
		{
		case (vk::PhysicalDeviceType::eCpu):
			std::cout << "CPU\n";
			break;
		case (vk::PhysicalDeviceType::eDiscreteGpu):
			std::cout << "Discrete GPU\n";
			break;
		case (vk::PhysicalDeviceType::eIntegratedGpu):
			std::cout << "Integrated GPU\n";
			break;
		case (vk::PhysicalDeviceType::eVirtualGpu):
			std::cout << "Virtual GPU\n";
			break;
		default:
			std::cout << "Other\n";
		}
	}

	std::vector<std::string> vkEngine::LogTransformBits(vk::SurfaceTransformFlagsKHR bits)
	{
		std::vector<std::string> result;
		if (bits & vk::SurfaceTransformFlagBitsKHR::eIdentity) result.push_back("Identity");
		if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate90) result.push_back("Rotate 90");
		if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate180) result.push_back("Rotate 180");
		if (bits & vk::SurfaceTransformFlagBitsKHR::eRotate270) result.push_back("Rotate 270");

		if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirror)
			result.push_back("Horizontal Mirror");
		if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate90)
			result.push_back("Horizontal Mirror & Rotate 90");
		if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate180)
			result.push_back("Horizontal Mirror & Rotate 180");
		if (bits & vk::SurfaceTransformFlagBitsKHR::eHorizontalMirrorRotate270)
			result.push_back("Horizontal Mirror & Rotate 270");

		if (bits & vk::SurfaceTransformFlagBitsKHR::eInherit) result.push_back("Inherited");

		return result;
	}
	std::vector<std::string> vkEngine::LogAlphaCompositeBits(vk::CompositeAlphaFlagsKHR bits)
	{
		std::vector<std::string> result;
		if (bits & vk::CompositeAlphaFlagBitsKHR::eOpaque) result.push_back("Opaque");
		if (bits & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) result.push_back("Pre-Multiplied");
		if (bits & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) result.push_back("Post-Multiplied");
		if (bits & vk::CompositeAlphaFlagBitsKHR::eInherit) result.push_back("Inherited");
		return result;
	}
	std::vector<std::string> vkEngine::LogImageUsageBits(vk::ImageUsageFlags bits)
	{
		std::vector<std::string> result;
		if (bits & vk::ImageUsageFlagBits::eTransferSrc) result.push_back("Transfer Source");
		if (bits & vk::ImageUsageFlagBits::eTransferDst) result.push_back("Transfer Destination");
		if (bits & vk::ImageUsageFlagBits::eSampled) result.push_back("Sampled");
		if (bits & vk::ImageUsageFlagBits::eStorage) result.push_back("Storage");

		if (bits & vk::ImageUsageFlagBits::eColorAttachment)
			result.push_back("Color Attachment");
		if (bits & vk::ImageUsageFlagBits::eDepthStencilAttachment)
			result.push_back("Depth Stencil Attachment");
		if (bits & vk::ImageUsageFlagBits::eTransientAttachment)
			result.push_back("Transient Attachment");
		if (bits & vk::ImageUsageFlagBits::eInputAttachment)
			result.push_back("Input Attachment");

		if (bits & vk::ImageUsageFlagBits::eFragmentDensityMapEXT)
			result.push_back("Fragment Density Map");
		if (bits & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR)
			result.push_back("Fragment Shading Rate Attachment");
		return result;
	}

	std::string vkEngine::LogPresentMode(vk::PresentModeKHR mode)
	{
		switch (mode)
		{
		case vk::PresentModeKHR::eImmediate:
			return "Immediate";
		case vk::PresentModeKHR::eMailbox:
			return "Mailbox";
		case vk::PresentModeKHR::eFifo:
			return "FIFO";
		case vk::PresentModeKHR::eFifoRelaxed:
			return "FIFO Relaxed";
		case vk::PresentModeKHR::eSharedDemandRefresh:
			return "Shared Demand Refresh";
		case vk::PresentModeKHR::eSharedContinuousRefresh:
			return "Shared Continuous Refresh";
		default:
			return "None/Unknown";
		}
	}
}