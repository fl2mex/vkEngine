#pragma once
#include <vulkan/vulkan.hpp> // Wish this was global...
#include <GLFW/glfw3.h>

#include "Swapchain.h"


class Engine
{
public:
	Engine(int width, int height, GLFWwindow* window, bool debug); // Constructor, no need to call a function
	~Engine(); // Destructor runs automatically when main loop ends

	void Render() const;

private:
	bool debugMode;
	uint32_t screenWidth;
	uint32_t screenHeight;
	GLFWwindow* m_Window;

	vk::Instance m_Instance{nullptr};
	vk::DebugUtilsMessengerEXT m_DebugMessenger{nullptr};
	vk::DispatchLoaderDynamic m_DLDI{};
	vk::SurfaceKHR m_Surface{};

	vk::PhysicalDevice m_PhysicalDevice{};
	vk::Device m_Device{nullptr};
	vk::Queue m_GraphicsQueue{nullptr}, m_PresentQueue{nullptr};

	vk::SwapchainKHR m_Swapchain{};
	std::vector<vkEngine::SwapchainFrame> m_SwapchainFrames{};
	vk::Format m_SwapchainFormat{};
	vk::Extent2D m_SwapchainExtent{};

	vk::PipelineLayout m_Layout;
	vk::RenderPass m_RenderPass;
	vk::Pipeline m_Pipeline;

	vk::CommandPool m_CommandPool;
	vk::CommandBuffer m_CommandBuffer;

	vk::Fence m_InFlightFence;
	vk::Semaphore m_ImageAvailable, m_RenderFinished;
};
