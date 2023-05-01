#include <iostream>

#include "Engine.h"

Engine::Engine()
{
	if (debugMode) std::cout << "Debug Mode Enabled\n";
	Initialize();
	instance = CreateInstance(true, "Vulkan App");
}

Engine::~Engine()
{
	if (debugMode) std::cout << "\nExitting Program..." << std::endl;
	instance.destroy();
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
	version = VK_MAKE_VERSION(0, 1, 0, 0);

	vk::ApplicationInfo appInfo(applicationName, version, "No Engine", version, version);
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (debug)
	{
		std::cout << "GLFW Extensions:\n";
		for (const char* extension : extensions)
			std::cout << "\t\"" << extension << "\"\n";
	}

	vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, 
		0, nullptr, static_cast<uint32_t>(extensions.size()), extensions.data());

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