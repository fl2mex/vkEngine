#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>

class Engine
{
public:
	Engine();
	~Engine();
private:
	void Initialize();
	vk::Instance CreateInstance(bool debug, const char* applicationName);

private:
	bool debugMode = true;
	unsigned int screenWidth = 1600;
	unsigned int screenHeight = 900;
	GLFWwindow* m_Window{nullptr};
	vk::Instance instance{nullptr};
};