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
		// 
		// week 03
		m_GradientShader.Initialize();
		m_RenderPass.CreateRenderPass(surface, window);
		m_GraphicsPipeline.CreateGraphicsPipeline(m_GradientShader, m_RenderPass.GetRenderPass());
		m_RenderPass.CreateFrameBuffers();
		//createRenderPass();
		//createGraphicsPipeline();
		//createFrameBuffers();

		// week 02
		m_CommandPool.Init(surface);
		//temp to remove later
		//m_VertexBuffer.CreateVertexBuffer();

		m_CommandBuffer.InitCommandBuffer(m_CommandPool.GetCommandPool());
		Scene::GetInstance().Init(m_CommandPool.GetCommandPool(), graphicsQueue);

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
		m_RenderPass.Destroy();

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

	//VkSwapchainKHR swapChain;

	//SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	//VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	//VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	//VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	//void createSwapChain();
	//void createImageViews();

	// Week 05 
	// Logical and physical device

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