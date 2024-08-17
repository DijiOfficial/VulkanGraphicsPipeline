#include "vulkanbase/VulkanBase.h"
//temp for vertcies
#include "mesh/Utils.h"
#include "core/Scene.h"
#include "core/InputManager.h"
#include "core/ResourceManager.h"

void VulkanBase::InitVulkan()
{
	m_Handles.InitializeVulkanHandles(window);

	m_GradientShader.Initialize();
	m_3DShader.Initialize();

	m_RenderPass.CreateRenderPass(m_Handles.GetSurface(), window);

	m_GraphicsPipeline.CreateGraphicsPipeline(m_GradientShader, m_RenderPass.GetRenderPass(), Vertex2D::CreateVertexInputStateInfo());
	m_3DGraphicsPipeline.CreateGraphicsPipeline(m_3DShader, m_RenderPass.GetRenderPass(), Vertex3D::CreateVertexInputStateInfo());
	m_RenderPass.CreateFrameBuffers();

	m_Camera.SetAspectRatio(m_RenderPass.GetAspectRatio());
	//m_Camera.Initialize(45.f, { 0.f, 0.f, -2.f });
	m_Camera.Initialize(45.0f, { 0.0f, 0.0f, -100.0f });

	m_CommandPool.Init(m_Handles.GetSurface());
	ResourceManager::GetInstance().Init(m_Handles.GetGraphicsQueue(), m_CommandPool.GetCommandPool(), "resources/textures/");

	m_CommandBuffer.InitCommandBuffer(m_CommandPool.GetCommandPool());

	Scene::GetInstance().Add3DDescriptorLayout(m_3DShader.GetDescriptorSetLayout());
	Scene::GetInstance().Init(m_CommandPool.GetCommandPool(), m_Handles.GetGraphicsQueue(), m_GradientShader.GetDescriptorSetLayout(), m_Camera.GetProjectionMatrix(), m_RenderPass.GetAspectRatio());

	m_Handles.InitializeSyncObjects();
}

void VulkanBase::Cleanup()
{
	m_Handles.DestroySemaphores();
	m_Handles.DestroyFence();

	m_CommandPool.DestroyCommandPool();
	Scene::GetInstance().Destroy();

	m_GraphicsPipeline.Destroy();
	m_3DGraphicsPipeline.Destroy();

	m_GradientShader.DestoryDescriptorSetLayout();
	m_3DShader.DestoryDescriptorSetLayout();
	m_RenderPass.Destroy();

	ResourceManager::GetInstance().Destroy();

	m_Handles.DestroyDebugMessenger();
	vkDestroyDevice(device, nullptr);

	m_Handles.DestroySurface();
	m_Handles.DestroyInstance();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void VulkanBase::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	InputManager::GetInstance().Init(window);
}

void VulkanBase::DrawFrame(uint32_t imageIndex)
{
	const VkCommandBuffer& commandBuffer = m_CommandBuffer.GetVkCommandBuffer();
	const auto& swapChainExtent = m_RenderPass.GetSwapChainExtent();

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_RenderPass.GetRenderPass();
	renderPassInfo.framebuffer = m_RenderPass.GetSwapChainFramebuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { { 0.2f,0.4f,0.6f,1.0f } };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline.GetGraphicsPipeline());
	Scene::GetInstance().Draw2DMeshes(commandBuffer, m_GraphicsPipeline.GetPipelineLayout(), imageIndex);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_3DGraphicsPipeline.GetGraphicsPipeline());
	Scene::GetInstance().Draw3DMeshes(commandBuffer, m_3DGraphicsPipeline.GetPipelineLayout(), imageIndex);

	vkCmdEndRenderPass(commandBuffer);
}

void VulkanBase::DrawFrame()
{
	vkWaitForFences(device, 1, &m_Handles.GetInFlightFence(), VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &m_Handles.GetInFlightFence());

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, m_RenderPass.GetSwapChain(), UINT64_MAX, m_Handles.GetImageAvailableSemaphore(), VK_NULL_HANDLE, &imageIndex);

	m_CommandBuffer.Reset();
	m_CommandBuffer.BeginFrame();

	vkCmdPushConstants(
		m_CommandBuffer.GetVkCommandBuffer(), m_3DGraphicsPipeline.GetPipelineLayout(),
		VK_SHADER_STAGE_FRAGMENT_BIT, // Stage flag should match the push constant range in the layout
		0, // Offset within the push constant block
		sizeof(glm::vec3), // Size of the push constants to update
		&m_Camera.GetPosition() // Pointer to the data
	);

	//vkCmdPushConstants(m_CommandBuffer.GetVkCommandBuffer(), GraphicsPipeline::GetPipelineLayout(),
	//	VK_SHADER_STAGE_FRAGMENT_BIT,
	//	sizeof(glm::vec3) + sizeof(int), sizeof(int), &m_ShadingMode);

	Scene::GetInstance().Update(m_Camera.GetInverseViewMatrix(), imageIndex);

	DrawFrame(imageIndex);
	m_CommandBuffer.EndFrame();


	VkSubmitInfo submitInfo{};
	VkSemaphore waitSemaphores[] = { m_Handles.GetImageAvailableSemaphore()};
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	VkSemaphore signalSemaphores[] = { m_Handles.GetRenderFinishedSemaphore()};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	m_CommandBuffer.SubmitInfo(submitInfo, m_Handles.GetGraphicsQueue(), m_Handles.GetInFlightFence());

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_RenderPass.GetSwapChain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(m_Handles.GetPresentQueue(), &presentInfo);
}