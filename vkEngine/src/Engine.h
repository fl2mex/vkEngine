#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <set>

class Engine
{
public:
	Engine();		
	~Engine();
private:
	bool debugMode = false;

	unsigned int screenWidth = 1600;
	unsigned int screenHeight = 900;
	GLFWwindow* m_Window{nullptr};

	vk::Instance m_Instance{nullptr};
	vk::DebugUtilsMessengerEXT m_DebugMessenger{nullptr};
	vk::DispatchLoaderDynamic m_DLDI{};
	vk::PhysicalDevice m_PhysicalDevice{nullptr};

	void Initialize();

	vk::Instance CreateInstance(bool debug, const char* applicationName);
	
	vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);
	
	vk::PhysicalDevice CreateDevice(const vk::Instance& instance, const bool debug);

};