#pragma once
#include "shaders/vulkan_shader.h"
#include "abstractions/CommandBuffer.h"
#include "abstractions/CommandPool.h"
#include "abstractions/RenderPass.h"
#include "abstractions/GraphicsPipeline.h"
#include "abstractions/Handles.h"
#include "core/Camera.h"
#include "core/TimeSingleton.h"
#include "core/RenderMode.h"
#include "core/Scene.h"
#include <chrono>

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

	void MainLoop() 
	{
		auto lastFrameTime{ std::chrono::high_resolution_clock::now() };

		while (!glfwWindowShouldClose(window)) 
		{
			const auto currentTime{ std::chrono::high_resolution_clock::now() };
			const float deltaTime{ std::chrono::duration<float>(currentTime - lastFrameTime).count() };
			lastFrameTime = currentTime;
			diji::TimeSingleton::GetInstance().SetDeltaTime(deltaTime);

			glfwPollEvents();

			m_RenderMode.Update();
			m_Camera.Update();
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
	Handles m_Handles{};
	Camera m_Camera{};
	RenderOptions m_RenderMode{};
	Scene m_Scene{};

	void InitWindow();
	void DrawFrame(uint32_t imageIndex);
	void DrawFrame();
};