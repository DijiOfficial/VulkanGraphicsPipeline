#pragma once
#include <vulkan/vulkan_core.h>

class VertexBuffer final
{
public:
	VertexBuffer() = default;
	explicit VertexBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDeviceSize& size, const void* data);
	~VertexBuffer() noexcept = default;

	//void CreateVertexBuffer();
	void Destroy() const;

	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& other) = delete;
	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(VertexBuffer&& other) = delete;

	VkBuffer GetVertexBuffer() const { return m_VertexBuffer; };
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

private:
	VkBuffer m_VertexBuffer;
	VkDeviceMemory m_VertexBufferMemory;

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	//void CreateVertexBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDeviceSize& size, const void* data);
	void CopyBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};