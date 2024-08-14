#pragma once
#include <vulkan/vulkan_core.h>
#include <optional>

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class CommandPool final
{
public:
	CommandPool() = default;
	~CommandPool() noexcept = default;

	void Init(const VkSurfaceKHR& surface);
	void DestroyCommandPool();

	CommandPool(const CommandPool& other) = delete;
	CommandPool(CommandPool&& other) = delete;
	CommandPool& operator=(const CommandPool& other) = delete;
	CommandPool& operator=(CommandPool&& other) = delete;

	const VkCommandPool& GetCommandPool() const { return m_CommandPool; };

	static QueueFamilyIndices FindQueueFamilies(const VkSurfaceKHR& surface, const VkPhysicalDevice& device);

private:
	VkCommandPool m_CommandPool{};
};

