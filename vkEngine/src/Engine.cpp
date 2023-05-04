#include <iostream>

#include "Engine.h"

Engine::Engine()
{
	if (debugMode) std::cout << "Debug Mode Enabled\n";
	Initialize();
	m_Instance = CreateInstance(true, "Vulkan App");
	m_DLDI = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);

	m_DebugMessenger = CreateDebugMessenger(m_Instance, m_DLDI);
}
Engine::~Engine()
{
	if (debugMode) std::cout << "\nExitting Program..." << std::endl;
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
	bool found;

	std::vector <vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
	if (debug)
	{
		std::cout << "Can support these extensions:\n";
		for (vk::ExtensionProperties supportedExtension : supportedExtensions)
			std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
	}
	for (const char* extension : extensions)
	{
		found = false;
		for (vk::ExtensionProperties supportedExtension : supportedExtensions)
		{
			if (strcmp(extension, supportedExtension.extensionName) == 0)
			{
				found = true;
				if (debug)
					std::cout << "Extension \"" << extension << "\" is supported\n";
			}
		}
		if (!found)
		{
			if (debug)
				std::cout << "Extension \"" << extension << "\" is not supported\n";
			return false;
		}
	}

	std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
	if (debug)
	{
		std::cout << "\nCan support these Layers:\n";
		for (vk::LayerProperties supportedLayer : supportedLayers)
			std::cout << "\t\"" << supportedLayer.layerName << "\"\n";
	}
	for (const char* layer : layers)
	{
		found = false;
		for (vk::LayerProperties supportedLayer : supportedLayers)
		{
			if (strcmp(layer, supportedLayer.layerName) == 0)
			{
				found = true;
				if (debug)
					std::cout << "Layer \"" << layer << "\" is supported\n";
			}
		}
		if (!found)
		{
			if (debug)
				std::cout << "Layer \"" << layer << "\" is not supported\n";
			return false;
		}
	}
	return true;
}
vk::Instance Engine::CreateInstance(bool debug, const char* applicationName)
{
	if (debug) std::cout << "\nCreating Vulkan Instance\n";
	uint32_t version{};
	vkEnumerateInstanceVersion(&version);
	if (debug)
		std::cout << "Vulkan Variant: " << VK_API_VERSION_VARIANT(version) <<
		"\nVersion: " << VK_API_VERSION_MAJOR(version) <<
		"." << VK_API_VERSION_MINOR(version) <<
		"." << VK_API_VERSION_PATCH(version) << "\n";
	version &= ~(0xFFFU);
	version = VK_MAKE_API_VERSION(0, 1, 0, 0);

	vk::ApplicationInfo appInfo(applicationName, version, "No Engine", version, version);
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	std::vector<const char*> layers;

	if (debug)
	{
		extensions.push_back("VK_EXT_debug_utils");
		layers.push_back("VK_LAYER_KHRONOS_validation");

		std::cout << "GLFW Extensions:\n";
		for (const char* extension : extensions)
			std::cout << "\t\"" << extension << "\"\n";
	};

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
	return nullptr;
}


VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData)
{
	std::cerr << "Validation Layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}
vk::DebugUtilsMessengerEXT Engine::CreateDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi)
{
	std::cout << "\nCreating Debug Messenger\n";
	vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
		vk::DebugUtilsMessengerCreateFlagsEXT(), vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
		vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance, debugCallback, nullptr);

	return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
}