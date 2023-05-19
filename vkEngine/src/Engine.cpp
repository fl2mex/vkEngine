#include "Engine.h"
#include "Device.h"
#include "Logging.h"
#include "Instance.h"
#include "Pipeline.h"
#include "FrameBuffer.h"
#include "Commands.h"
#include "Synchronisation.h"

#include <iostream>

Engine::Engine(const int width, const int height, GLFWwindow* window, const bool debug)
{
	screenWidth = width;
	screenHeight = height;
	m_Window = window;
	debugMode = debug;
	if (debugMode) std::cout << "Debug Mode Enabled\n";

	m_Instance = vkEngine::CreateInstance(debugMode, "Vulkan App"); // Create Window

	m_DLDI = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);
	if (debugMode) m_DebugMessenger = vkEngine::CreateDebugMessenger(m_Instance, m_DLDI);

	VkSurfaceKHR surface;
	if (glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &surface) != VK_SUCCESS)
	{
		if (debugMode) std::cout << "Failed to create a Window Surface\n";
	}
	else if (debugMode) std::cout << "Created a Window Surface\n";
	m_Surface = surface;

	m_PhysicalDevice = vkEngine::CreateDevice(m_Instance, debugMode);
	m_Device = vkEngine::CreateLogicalDevice(m_PhysicalDevice, m_Surface, debugMode);

	const std::array<vk::Queue, 2> queues = vkEngine::GetQueue(m_PhysicalDevice, m_Device, m_Surface, debugMode);
	m_GraphicsQueue = queues[0];
	m_PresentQueue = queues[1];

	const vkEngine::SwapchainBundle bundle = vkEngine::CreateSwapchain(m_Device, m_PhysicalDevice,
	                                                                   m_Surface, screenWidth, screenHeight, debugMode);
	m_Swapchain = bundle.swapchain;
	m_SwapchainFrames = bundle.frames;
	m_SwapchainFormat = bundle.format;
	m_SwapchainExtent = bundle.extent;

	vkEngine::GraphicsPipelineIn specs{};
	specs.device = m_Device;
	specs.vertexFilePath = "res/shader/compiled/vert.spv";
	specs.fragmentFilePath = "res/shader/compiled/frag.spv";
	specs.swapchainExtent = m_SwapchainExtent;
	specs.swapchainImageFormat = m_SwapchainFormat;
	const vkEngine::GraphicsPipelineOut output = CreateGraphicsPipeline(specs, debugMode);

	m_Layout = output.layout;
	m_RenderPass = output.renderPass;
	m_Pipeline = output.pipeline;

	vkEngine::FrameBufferInput frameBufferInput;
	frameBufferInput.device = m_Device;
	frameBufferInput.renderPass = m_RenderPass;
	frameBufferInput.swapchainExtent = m_SwapchainExtent;
	vkEngine::CreateFrameBuffers(frameBufferInput, m_SwapchainFrames, debugMode);

	m_CommandPool = vkEngine::CreateCommandPool(m_Device, m_PhysicalDevice, m_Surface, debugMode);

	const vkEngine::CommandBufferInputChunk commandBufferInput = {m_Device, m_CommandPool, m_SwapchainFrames};
	m_CommandBuffer = CreateCommandBuffers(commandBufferInput, debugMode);

	m_InFlightFence = vkEngine::CreateFence(m_Device, debugMode);
	m_ImageAvailable = vkEngine::CreateSemaphore(m_Device, debugMode);
	m_RenderFinished = vkEngine::CreateSemaphore(m_Device, debugMode);
}

Engine::~Engine()
{
	m_Device.waitIdle();

	std::cout << "Exitting Program...\n";

	m_Device.destroyFence(m_InFlightFence);
	m_Device.destroySemaphore(m_ImageAvailable);
	m_Device.destroySemaphore(m_RenderFinished);

	m_Device.destroyCommandPool(m_CommandPool);

	m_Device.destroyPipeline(m_Pipeline);
	m_Device.destroyPipelineLayout(m_Layout);
	m_Device.destroyRenderPass(m_RenderPass);

	for (const vkEngine::SwapchainFrame frame : m_SwapchainFrames)
	{
		m_Device.destroyImageView(frame.imageView);
		m_Device.destroyFramebuffer(frame.framebuffer);
	}

	m_Device.destroySwapchainKHR(m_Swapchain);
	m_Device.destroy();

	if (debugMode) m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_DLDI);
	m_Instance.destroySurfaceKHR(m_Surface);
	m_Instance.destroy();
	glfwTerminate();
}

void Engine::Render() const
{
	m_Device.waitForFences(1, &m_InFlightFence, VK_TRUE, UINT64_MAX);
	m_Device.resetFences(1, &m_InFlightFence);

	const uint32_t imageIndex{m_Device.acquireNextImageKHR(m_Swapchain, UINT64_MAX, m_ImageAvailable, nullptr).value};

	const vk::CommandBuffer commandBuffer = m_SwapchainFrames[imageIndex].commandBuffer;
	commandBuffer.reset();

	constexpr vk::CommandBufferBeginInfo beginInfo = {};

	try
	{
		commandBuffer.begin(beginInfo);
	}
	catch (vk::SystemError& err)
	{
		if (debugMode) std::cout << "Failed to begin recording Command Buffer" << err.what() << "\n";
	}

	vk::RenderPassBeginInfo renderPassInfo{};
	renderPassInfo.renderPass = m_RenderPass;
	renderPassInfo.framebuffer = m_SwapchainFrames[imageIndex].framebuffer;
	renderPassInfo.renderArea.offset.x = 0;
	renderPassInfo.renderArea.offset.y = 0;
	renderPassInfo.renderArea.extent = m_SwapchainExtent;

	const vk::ClearValue clearColor = {std::array{1.0f, 0.5f, 0.25f, 1.0f}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);
	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, m_Pipeline);
	commandBuffer.draw(3, 1, 0, 0);
	commandBuffer.endRenderPass();

	try
	{
		commandBuffer.end();
	}
	catch (vk::SystemError& err)
	{
		if (debugMode) std::cout << "Failed to record Command Buffer\n" << err.what() << "\n";
	}

	vk::SubmitInfo submitInfo = {};
	const vk::Semaphore waitSemaphores[] = {m_ImageAvailable};
	constexpr vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	const vk::Semaphore signalSemaphores[] = {m_RenderFinished};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	try
	{
		m_GraphicsQueue.submit(submitInfo, m_InFlightFence);
	}
	catch (vk::SystemError& err)
	{
		if (debugMode) std::cout << "Failed to submit Draw Command Buffer\n" << err.what() << "\n";
	}

	vk::PresentInfoKHR presentInfo = {};
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	const vk::SwapchainKHR swapChains[] = {m_Swapchain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	m_PresentQueue.presentKHR(presentInfo);
}
