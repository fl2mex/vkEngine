#include "FrameBuffer.h"

#include <iostream>

namespace vkEngine
{
	void vkEngine::CreateFrameBuffers(FrameBufferInput input, std::vector<vkEngine::SwapchainFrame>& frames, bool debug)
	{
		for (int i = 0; i < frames.size(); i++)
		{
			std::vector<vk::ImageView> attachments = { frames[i].imageView };

			vk::FramebufferCreateInfo frameBufferInfo = {};
			frameBufferInfo.flags = vk::FramebufferCreateFlags();
			frameBufferInfo.renderPass = input.renderPass;
			frameBufferInfo.attachmentCount = attachments.size();
			frameBufferInfo.pAttachments = attachments.data();
			frameBufferInfo.width = input.swapchainExtent.width;
			frameBufferInfo.height = input.swapchainExtent.height;
			frameBufferInfo.layers = 1;

			try
			{
				frames[i].framebuffer = input.device.createFramebuffer(frameBufferInfo);
				if (debug) std::cout << "Framebuffer created for frame " << i << std::endl;
			}
			catch (vk::SystemError& err)
			{
				if (debug) std::cout << "Framebuffer creation failed for frame "<<
					i << "\n" << err.what() << std::endl;
			}
			
		}
	}
}