#pragma once
#include "vulkanbase/VulkanUtil.h"
#include <iostream>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class Handles final
{
public:
	Handles() = default;
	~Handles() noexcept = default;

	Handles(const Handles& other) = delete;
	Handles(Handles&& other) = delete;
	Handles& operator=(const Handles& other) = delete;
	Handles& operator=(Handles&& other) = delete;

	void InitializeVulkanHandles(GLFWwindow* window);
	void InitializeSyncObjects();

	void DestroySemaphores();
	void DestroyFence();
	void DestroyDebugMessenger();
	void DestroySurface();
	void DestroyInstance();

	const VkSurfaceKHR& GetSurface() { return surface; }
	const VkQueue& GetGraphicsQueue() { return graphicsQueue; }
	const VkQueue& GetPresentQueue() { return presentQueue; }
	const VkFence& GetInFlightFence() { return inFlightFence; }
	const VkSemaphore& GetImageAvailableSemaphore() { return imageAvailableSemaphore; }
	const VkSemaphore& GetRenderFinishedSemaphore() { return renderFinishedSemaphore; }

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

private:

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	std::vector<const char*> GetRequiredExtensions();

	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateInstance();

	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckValidationLayerSupport();

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void SetupDebugMessenger();
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	void CreateSyncObjects();
	void CreateSurface(GLFWwindow* window);
	
};
