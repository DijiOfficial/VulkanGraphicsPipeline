#pragma once
#include <vulkan/vulkan_core.h>

class DataBuffer final
{
public:
	DataBuffer() = default;
	explicit DataBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDeviceSize& size, const void* data, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
	~DataBuffer() noexcept = default;

	void Destroy() const;

	DataBuffer(const DataBuffer& other) = delete;
	DataBuffer(DataBuffer&& other) = delete;
	DataBuffer& operator=(const DataBuffer& other) = delete;
	DataBuffer& operator=(DataBuffer&& other) = delete;

	VkBuffer GetVertexBuffer() const { return m_VertexBuffer; };
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CopyBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};