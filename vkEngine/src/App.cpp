#include "App.h"

#include <iostream>
#include <sstream>

App::App(const int width, const int height, const bool debug)
{
	CreateWindow(width, height, debug);
	m_GraphicsEngine = new Engine(width, height, m_Window, debug);
}

void App::CreateWindow(const int width, const int height, const bool debugMode)
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // This will be changed later
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	m_Window = glfwCreateWindow(width, height, "Vulkan App", nullptr, nullptr);
	if (m_Window)
	{
		std::cout << "Created a GLFW Window called \"Vulkan App\""
			"\nResolution is " << width << "x" << height << "\n";
	}
	else if (debugMode)
	{
		std::cout << "Failed to create a GLFW Window\n";
	}
}

void App::Run()
{
	while (!glfwWindowShouldClose(m_Window))
	{
		glfwPollEvents();
		m_GraphicsEngine->Render();
		CalculateFrameRate();
	}
}

void App::CalculateFrameRate()
{
	m_CurrentTime = glfwGetTime();
	double deltaTime = m_CurrentTime - m_LastTime;

	if (deltaTime >= 1)
	{
		m_FPS = std::max(1, static_cast<int>(m_NumFrames / deltaTime));
		std::stringstream title;
		title << "Vulkan App - Running at " << m_FPS << "FPS";
		glfwSetWindowTitle(m_Window, title.str().c_str());
		m_LastTime = m_CurrentTime;
		m_NumFrames = -1;
		m_FrameTime = 1000.0f / static_cast<float>(m_FPS);
	}
	m_NumFrames++;
}

App::~App()
{
	delete m_GraphicsEngine;
}
