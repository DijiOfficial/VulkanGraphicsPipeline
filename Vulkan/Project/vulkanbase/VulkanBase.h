#pragma once
#include "shaders/vulkan_shader.h"
#include "abstractions/CommandBuffer.h"
#include "abstractions/CommandPool.h"
#include "abstractions/RenderPass.h"
#include "abstractions/GraphicsPipeline.h"
#include "abstractions/Handles.h"

class VulkanBase {
public:
	void run() {
		InitWindow();
		InitVulkan();
		MainLoop();
		Cleanup();
	}

	inline static VkDevice device = VK_NULL_HANDLE;
	inline static VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

private:
	void InitVulkan();
	void Cleanup();

	void MainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			// week 06
			DrawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	GLFWwindow* window;
	VulkanShader m_GradientShader{ "shaders/shader.vert.spv", "shaders/shader.frag.spv" };
	VulkanShader m_3DShader{ "shaders/3Dshader.vert.spv", "shaders/3Dshader.frag.spv" };
	CommandBuffer m_CommandBuffer{};
	CommandPool m_CommandPool{};
	RenderPass m_RenderPass{};
	GraphicsPipeline m_GraphicsPipeline{};
	GraphicsPipeline m_3DGraphicsPipeline{};
	Handles m_Handles;


	void InitWindow();
	void DrawFrame(uint32_t imageIndex);
	void DrawFrame();
};