#pragma once
#include <vulkan/vulkan_core.h>
#include "GLFW/glfw3.h"
#include "SwapChainImageView.h"
#include "DepthBuffer.h"

#include <vector>

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain final
{
public:
	SwapChain() = default;
	~SwapChain() noexcept = default;

	SwapChain(const SwapChain& other) = delete;
	SwapChain(SwapChain&& other) = delete;
	SwapChain& operator=(const SwapChain& other) = delete;
	SwapChain& operator=(SwapChain&& other) = delete;

	void CreateSwapChain(const VkSurfaceKHR& surface, GLFWwindow* window, VkExtent2D& swapChainExtent);
	void Destroy();

	VkSwapchainKHR& GetSwapChain() { return m_SwapChain; }
	SwapChainImageView& GetImageView() { return m_ImageView; }

private:
	VkSwapchainKHR m_SwapChain{};
	SwapChainImageView m_ImageView{};

	SwapChainSupportDetails QuerySwapChainSupport(const VkSurfaceKHR& surface);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};

class RenderPass final 
{
public:
	RenderPass() = default;
	~RenderPass() noexcept = default;

	RenderPass(const RenderPass& other) = delete;
	RenderPass(RenderPass&& other) = delete;
	RenderPass& operator=(const RenderPass& other) = delete;
	RenderPass& operator=(RenderPass&& other) = delete;

	void CreateFrameBuffers();
	void CreateRenderPass(const VkSurfaceKHR& surface, GLFWwindow* window);
	void Destroy();

	float GetAspectRatio() const { return m_SwapChainExtent.width / static_cast<float>(m_SwapChainExtent.height); }
	VkRenderPass& GetRenderPass() { return m_RenderPass; }
	std::vector<VkFramebuffer>& GetSwapChainFramebuffers() { return m_SwapChainFramebuffers; }
	VkExtent2D& GetSwapChainExtent() { return m_SwapChainExtent; }
	VkSwapchainKHR& GetSwapChain() { return m_SwapChainHandle.GetSwapChain(); }
private:

	VkRenderPass m_RenderPass{};
	std::vector<VkFramebuffer> m_SwapChainFramebuffers{};
	VkExtent2D m_SwapChainExtent{};

	SwapChain m_SwapChainHandle{};
	DepthBuffer m_DepthBuffer{};
};