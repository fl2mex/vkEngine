#include <iostream>

#include "Engine.h"


Engine::Engine()
{
	if (debugMode) std::cout << "Debug Mode Enabled\n";
	Initialize();
	m_Instance = CreateInstance(debugMode, "Vulkan App");

	m_DLDI = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
	if (debugMode) m_DebugMessenger = CreateDebugMessenger(m_Instance, m_DLDI);

	m_PhysicalDevice = CreateDevice(m_Instance, debugMode);
}
Engine::~Engine()
{
	if (debugMode) std::cout << "\nExitting Program..." << std::endl;
	if (debugMode) m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DLDI);
	m_Instance.destroy();
	glfwTerminate();
}
void Engine::Initialize()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_Window = glfwCreateWindow(screenWidth, screenHeight, "Vulkan App", nullptr, nullptr);
	if (m_Window)
	{
		std::cout << "Created a GLFW Window called \"Vulkan App\""
			"\nResolution is " << screenWidth << "x" << screenHeight << std::endl;
		return;
	}
	else if (debugMode)
	{
		std::cout << "Failed to create a GLFW Window" << std::endl;
		return;
	}
}


bool isSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug)
{
	std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
	if (debug)
	{
		std::cout << "Device can support the following extensions:\n";
		for (vk::ExtensionProperties supportedExtension : supportedExtensions)
			std::cout << '\t' << supportedExtension.extensionName << '\n';
	}

	bool found;
	for (const char* extension : extensions)
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
			if (debug) std::cout << "Extension \"" << extension << "\" is not supported!\n";
			return false;
		}
	}

	std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
	if (debug)
	{
		std::cout << "Device can support the following layers:\n";
		for (vk::LayerProperties supportedLayer : supportedLayers)
			std::cout << '\t' << supportedLayer.layerName << '\n';
	}

	for (const char* layer : layers)
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
			if (debug) std::cout << "Layer \"" << layer << "\" is not supported!\n";
			return false;
		}
	}
	return true;
}
vk::Instance Engine::CreateInstance(bool debug, const char* applicationName)
{
	if (debug) std::cout << "\nCreating Vulkan Instance\n";
	uint32_t version{0};
	vkEnumerateInstanceVersion(&version);
	if (debug)
		std::cout << "Vulkan Variant: " << 
		VK_API_VERSION_VARIANT(version) << "\nVersion: " << VK_API_VERSION_MAJOR(version) <<
		"." << VK_API_VERSION_MINOR(version) << "." << VK_API_VERSION_PATCH(version) << "\n";

	version &= ~(0xFFFU);
	version = VK_MAKE_API_VERSION(0, 1, 0, 0);

	vk::ApplicationInfo appInfo(applicationName, version, "No Engine", version, version);

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if (debug) extensions.push_back("VK_EXT_debug_utils");
	if (debug)
	{
		std::cout << "Extensions requested:\n";
		for (const char* extensionName : extensions)
			std::cout << "\t\"" << extensionName << "\"\n";
	}

	std::vector<const char*> layers;
	if (debug) layers.push_back("VK_LAYER_KHRONOS_validation");
	
	if (!isSupported(extensions, layers, debug)) return nullptr;

	vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo,
		static_cast<uint32_t>(layers.size()), layers.data(),
		static_cast<uint32_t>(extensions.size()), extensions.data());

	try
	{
		vk::Instance instance = vk::createInstance(createInfo);
		if (debug) std::cout << "Created Vulkan Instance" << std::endl;
		return instance;
	}
	catch (vk::SystemError err)
	{
		std::cout << "Failed to create Vulkan Instance\n";
		std::cout << err.what() << std::endl;
		return nullptr;
	}
}


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}

vk::DebugUtilsMessengerEXT Engine::CreateDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi)
{
	vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(vk::DebugUtilsMessengerCreateFlagsEXT(),
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		debugCallback, nullptr);
	return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
}

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
	return requiredExtensions.empty();
}

bool isSuitable(const vk::PhysicalDevice& device, const bool debug) {

	if (debug) std::cout << "Checking if device is suitable\n";

	const std::vector<const char*> requestedExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	if (debug)
	{
		std::cout << "We are requesting device extensions:\n";
		for (const char* extension : requestedExtensions)
			std::cout << "\t\"" << extension << "\"\n";
	}

	if (bool extensionsSupported = checkDeviceExtensionSupport(device, requestedExtensions, debug))
		if (debug) std::cout << "Device can support the requested extensions!\n";
	else
	{
		if (debug) std::cout << "Device can't support the requested extensions!\n";
		return false;
	}
	return true;
}

vk::PhysicalDevice Engine::CreateDevice(const vk::Instance& instance, const bool debug)
{
	if (debugMode) std::cout << "Choosing Physical Device\n";
	std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
	if (debugMode) std::cout << "There are " << availableDevices.size() << " physical devices available on this system\n";

	for (vk::PhysicalDevice device : availableDevices)
	{
		if (debug)
		{
			vk::PhysicalDeviceProperties properties = device.getProperties();
			std::cout << "Device name: " << properties.deviceName << '\n';
			std::cout << "Device type: ";
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

		if (isSuitable(device, debug))
		{
			return device;
		}
	}
	return nullptr;
}