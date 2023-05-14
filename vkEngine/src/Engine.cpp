#include "Engine.h"
#include "Device.h"
#include "Logging.h"
#include "Instance.h"

#include <iostream>

Engine::Engine()
{
	if (debugMode) std::cout << "Debug Mode Enabled\n";

	Initialize();

	m_Instance = vkEngine::CreateInstance(debugMode, "Vulkan App"); // Create Window
	
	m_DLDI = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
	// TODO: Fix this
	//if (debugMode) m_DebugMessenger = vkEngine::CreateDebugMessenger(m_Instance, m_DLDI);

	VkSurfaceKHR surface;

	if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &surface) != VK_SUCCESS)
		if (debugMode) 
		{
			std::cout << "Failed to create a Window Surface\n";
			return;
		}
	else if (debugMode) std::cout << "Created a Window Surface\n";
	m_Surface = surface;
	
	m_PhysicalDevice = vkEngine::CreateDevice(m_Instance, debugMode);
	m_Device = vkEngine::CreateLogicalDevice(m_PhysicalDevice, m_Surface, debugMode);

	std::array<vk::Queue, 2> queues = vkEngine::GetQueue(m_PhysicalDevice, m_Device, m_Surface, debugMode);
	m_GraphicsQueue = queues[0];
	m_PresentQueue = queues[1];
	vkEngine::SwapchainBundle bundle = vkEngine::CreateSwapchain(m_Device, m_PhysicalDevice,
		m_Surface, screenWidth, screenHeight, debugMode);
	m_Swapchain = bundle.swapchain;
	m_SwapchainFrames = bundle.frames;
	m_SwapchainFormat = bundle.format;
	m_SwapchainExtent = bundle.extent;

	//vkEngine::QuerySwapSupport(m_PhysicalDevice, m_Surface, debugMode);
}

Engine::~Engine()
{
	std::cout << "Exitting Program...\n";

	for (vkEngine::SwapchainFrame frame : m_SwapchainFrames)
		m_Device.destroyImageView(frame.imageView);
	m_Device.destroySwapchainKHR(m_Swapchain);
	m_Device.destroy();
	if (debugMode) m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DLDI);
	m_Instance.destroySurfaceKHR(m_Surface);
	m_Instance.destroy();
	glfwTerminate();
}

void Engine::Initialize()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
		return;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // This will be changed later
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	m_Window = glfwCreateWindow(screenWidth, screenHeight, "Vulkan App", nullptr, nullptr);
	if (m_Window)
	{
		std::cout << "Created a GLFW Window called \"Vulkan App\""
			"\nResolution is " << screenWidth << "x" << screenHeight << "\n";
		return;
	}
	else if (debugMode)
	{
		std::cout << "Failed to create a GLFW Window\n";
		return;
	}
}