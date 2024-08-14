#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "VulkanUtil.h"
#include "shaders/vulkan_shader.h"

#include "CommandBuffer.h"
#include "CommandPool.h"
#include "mesh/Utils.h"
#include "DataBuffer.h"
#include "Scene.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"

#include <iostream>
//#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
//#include <optional>
#include <set>
#include <limits>
#include <algorithm>

//temp
#include "textures/Textures.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

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
	void InitVulkan() {
		// week 06
		createInstance();
		setupDebugMessenger();
		CreateSurface();

		// week 05
		pickPhysicalDevice();
		createLogicalDevice();

		// week 04 
		//createSwapChain();
		//createImageViews();//to replace yet
		//swapChain and Image view handled by RenderPass
		
		
		// week 03
		m_GradientShader.Initialize();
		m_RenderPass.CreateRenderPass(surface, window);
		m_GraphicsPipeline.CreateGraphicsPipeline(m_GradientShader, m_RenderPass.GetRenderPass());
		m_RenderPass.CreateFrameBuffers();
		//createRenderPass();
		//createGraphicsPipeline();
		//createFrameBuffers();
		//CreateUniformBuffers();

		// week 02
		m_CommandPool.Init(surface);
		m_Texture.CreateTextureImage(graphicsQueue, m_CommandPool.GetCommandPool(), "pooop");
		m_Texture.CreateTextureSampler();

		//temp to remove later
		//m_VertexBuffer.CreateVertexBuffer();

		m_CommandBuffer.InitCommandBuffer(m_CommandPool.GetCommandPool());
		Scene::GetInstance().Init(m_CommandPool.GetCommandPool(), graphicsQueue, m_GradientShader.GetDescriptorSetLayout(), m_RenderPass.GetAspectRatio());

		// week 06
		createSyncObjects();
	}

	void MainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			// week 06
			drawFrame();
		}
		vkDeviceWaitIdle(device);
	}

	void Cleanup() {
		vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
		vkDestroyFence(device, inFlightFence, nullptr);

		//m_VertexBuffer.Destroy(); // todo: can I put destroy in destructor?
		m_CommandPool.DestroyCommandPool();
		Scene::GetInstance().Destroy();

		m_GraphicsPipeline.Destroy();

		m_GradientShader.DestoryDescriptorSetLayout(); //might not work if object is destoryed, but I destory the shader not the object
		m_RenderPass.Destroy();

		m_Texture.Destroy();

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}
		vkDestroyDevice(device, nullptr);

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void CreateSurface() {
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	VulkanShader m_GradientShader{ "shaders/shader.vert.spv", "shaders/shader.frag.spv" };
	//VulkanShader m_GradientShader{ "shaders/3Dshader.vert.spv", "shaders/shader.frag.spv" };

	GLFWwindow* window;
	void InitWindow();

	// Week 02
	// Queue families
	// CommandBuffer concept

	CommandBuffer m_CommandBuffer{};
	CommandPool m_CommandPool{};
	//temp
	//VertexBuffer m_VertexBuffer{};

	void DrawFrame(uint32_t imageIndex);
	
	// Week 03
	// Renderpass concept
	// Graphics pipeline
	
	
	
	RenderPass m_RenderPass{};
	GraphicsPipeline m_GraphicsPipeline{};
	//void createFrameBuffers();
	//void createRenderPass();
	//void createGraphicsPipeline();

	// Week 04
	// Swap chain and image view support
	//add to DataBuffer and remove static
	//std::vector<VkBuffer> uniformBuffers;
	//std::vector<VkDeviceMemory> uniformBuffersMemory;
	//std::vector<void*> uniformBuffersMapped;
	//UniformBufferObject ubo{};

	//void CreateUniformBuffers()
	//{
	//	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	//	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	//	uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	//	uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	//	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	//	{
	//		//DataBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

	//		vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	//	}
	//}

	//void updateUniformBuffer(uint32_t currentFrame)
	//{
	//	static auto startTime = std::chrono::high_resolution_clock::now();

	//	auto currentTime = std::chrono::high_resolution_clock::now();
	//	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	//	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//	ubo.proj = glm::perspective(glm::radians(45.0f),  m_RenderPass.GetAspectRatio(), 0.1f, 10.0f);
	//	ubo.proj[1][1] *= -1;

	//	memcpy(uniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));

	//}
	//VkSwapchainKHR swapChain;

	//SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	//VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	//VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	//VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	//void createSwapChain();
	//void createImageViews();

	// Week 05 
	// Logical and physical device

	//temp
	Texture m_Texture;

	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	void createLogicalDevice();

	// Week 06
	// Main initialization

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	
	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void setupDebugMessenger();
	std::vector<const char*> getRequiredExtensions();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	void createInstance();

	void createSyncObjects();
	void drawFrame();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};