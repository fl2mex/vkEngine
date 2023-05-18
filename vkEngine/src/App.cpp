#include "App.h"

#include <sstream>

App::App(int width, int height, bool debug)
{
	CreateWindow(width, height, debug);
	m_GraphicsEngine = new Engine(width, height, m_Window, debug);
}

void App::CreateWindow(int width, int height, bool debugMode)
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
		int fps = std::max(1, int(m_NumFrames / deltaTime));
		std::stringstream title; title << "Vulkan App - Running at " << fps << "FPS";
		glfwSetWindowTitle(m_Window, title.str().c_str());
		m_LastTime = m_CurrentTime;
		m_NumFrames = -1;
		m_FrameTime = 1000.0f / fps;
	}
	m_NumFrames++;
}

App::~App()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
	delete m_GraphicsEngine;
}