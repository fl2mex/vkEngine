#pragma once
#include "Engine.h"

class App
{
public:
	App(int width, int height, bool debug);
	~App();
	void Run();

private:
	Engine* m_GraphicsEngine;
	GLFWwindow* m_Window;

	int m_FPS;
	double m_LastTime, m_CurrentTime;
	int m_NumFrames;
	float m_FrameTime;

	void CreateWindow(int width, int height, bool debugMode);

	void CalculateFrameRate();
};
