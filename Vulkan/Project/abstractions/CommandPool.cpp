#include "CommandPool.h"
#include "vulkanbase\VulkanBase.h"

void CommandPool::Init(const VkSurfaceKHR& surface)
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(surface, VulkanBase::physicalDevice);

	VkCommandPool commandPool{};
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(VulkanBase::device, &poolInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void CommandPool::DestroyCommandPool()
{
	vkDestroyCommandPool(VulkanBase::device, m_CommandPool, nullptr);
}

QueueFamilyIndices CommandPool::FindQueueFamilies(const VkSurfaceKHR& surface, const VkPhysicalDevice& device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}
