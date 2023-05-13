#pragma once
#include <vulkan/vulkan.hpp> // Wish this was global...
#include <GLFW/glfw3.h>

class Engine
{
public:
	Engine(); // Constructor, no need to call a function
	~Engine(); // Destructor runs automatically when main loop ends
private:
	bool debugMode = true;

	uint32_t screenWidth = 1600;
	uint32_t screenHeight = 900;
	GLFWwindow* m_Window{nullptr};

	vk::Instance m_Instance{nullptr};
	vk::DebugUtilsMessengerEXT m_DebugMessenger{nullptr};
	vk::DispatchLoaderDynamic m_DLDI{};
	vk::SurfaceKHR m_Surface{};

	vk::PhysicalDevice m_PhysicalDevice{nullptr};
	vk::Device m_Device{nullptr};
	vk::Queue m_GraphicsQueue{nullptr};
	vk::Queue m_PresentQueue{nullptr};

	void Initialize();
	// void MainLoop(); // TODO: Create Main Loop
};