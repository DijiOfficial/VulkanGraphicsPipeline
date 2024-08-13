#pragma once
#include <vulkan/vulkan_core.h>

class DataBuffer final
{
public:
	DataBuffer() = default;
	explicit DataBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDeviceSize& size, const void* data, VkBufferUsageFlags usage);
	~DataBuffer() noexcept = default;

	void Destroy() const;

	DataBuffer(const DataBuffer& other) = delete;
	DataBuffer(DataBuffer&& other) = delete;
	DataBuffer& operator=(const DataBuffer& other) = delete;
	DataBuffer& operator=(DataBuffer&& other) = delete;

	VkBuffer& GetDataBuffer()  { return m_DataBuffer; };
	VkDeviceMemory& GetDataBufferMemory()  { return m_DataBufferMemory; };
	static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:
	VkBuffer m_DataBuffer;
	VkDeviceMemory m_DataBufferMemory;

	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void CopyBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};