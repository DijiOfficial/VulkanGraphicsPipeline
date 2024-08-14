#pragma once
#include <vulkan/vulkan_core.h>

class CommandBuffer final
{
public:
	CommandBuffer() = default;
	~CommandBuffer() noexcept = default;
	explicit CommandBuffer(const VkCommandPool& commandPool);

	void InitCommandBuffer(const VkCommandPool& commandPool);
	void Destroy(const VkCommandPool& commandPool) const;

	CommandBuffer(const CommandBuffer& other) = delete;
	CommandBuffer(CommandBuffer&& other) = delete;
	CommandBuffer& operator=(const CommandBuffer& other) = delete;
	CommandBuffer& operator=(CommandBuffer&& other) = delete;

	void Reset() const;
	void BeginFrame(const VkCommandBufferUsageFlags& flags = 0) const;
	void EndFrame() const;

	[[nodiscard]] const VkCommandBuffer& GetVkCommandBuffer() const { return m_CommandBuffer; };
	void SetVkCommandBuffer(VkCommandBuffer buffer) { m_CommandBuffer = buffer; }

	void SubmitInfo(VkSubmitInfo submitInfo, const VkQueue& graphicsQueue, const VkFence& fence) const;

private:
	VkCommandBuffer m_CommandBuffer{};

};