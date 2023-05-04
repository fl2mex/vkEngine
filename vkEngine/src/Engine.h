#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class Engine
{
public:
	Engine();
	~Engine();
private:
	bool debugMode = true;
	unsigned int screenWidth = 1600;
	unsigned int screenHeight = 900;

	GLFWwindow* m_Window{ nullptr };

	vk::Instance m_Instance{ nullptr };
	vk::DebugUtilsMessengerEXT m_DebugMessenger{ nullptr };
	vk::DispatchLoaderDynamic m_DLDI{ nullptr };

	void Initialize();
	vk::Instance CreateInstance(bool debug, const char* applicationName);
	vk::DebugUtilsMessengerEXT CreateDebugMessenger(vk::Instance& instance, vk::DispatchLoaderDynamic& dldi);
};