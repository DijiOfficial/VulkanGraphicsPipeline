#include "DataBuffer.h"
#include <vulkanbase/VulkanBase.h>

DataBuffer::DataBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDeviceSize& size, const void* data, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, properties, stagingBuffer, stagingBufferMemory);

	void* tempData;
	vkMapMemory(VulkanBase::device, stagingBufferMemory, 0, size, 0, &tempData);
	memcpy(tempData, data, (size_t)size);
	vkUnmapMemory(VulkanBase::device, stagingBufferMemory);

	CreateBuffer(size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_VertexBuffer, m_VertexBufferMemory);

	CopyBuffer(commandPool, graphicsQueue, stagingBuffer, m_VertexBuffer, size);

	vkDestroyBuffer(VulkanBase::device, stagingBuffer, nullptr);
	vkFreeMemory(VulkanBase::device, stagingBufferMemory, nullptr);
}

void DataBuffer::Destroy() const
{
	vkDestroyBuffer(VulkanBase::device, m_VertexBuffer, nullptr);
	vkFreeMemory(VulkanBase::device, m_VertexBufferMemory, nullptr);
}

void DataBuffer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(VulkanBase::device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(VulkanBase::device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(VulkanBase::device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(VulkanBase::device, buffer, bufferMemory, 0);
}

void DataBuffer::CopyBuffer(VkCommandPool const& commandPool, VkQueue const& graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	CommandBuffer commandBufferClass{ commandPool };

	commandBufferClass.BeginFrame(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBufferClass.GetVkCommandBuffer(), srcBuffer, dstBuffer, 1, &copyRegion);

	commandBufferClass.EndFrame();

	VkSubmitInfo submitInfo{};
	commandBufferClass.SubmitInfo(submitInfo);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	vkQueueWaitIdle(graphicsQueue);

	commandBufferClass.Destroy(commandPool);
}

uint32_t DataBuffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(VulkanBase::physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}