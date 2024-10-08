#pragma once
#include <vulkan/vulkan_core.h>
#include <string>

class Texture final
{
public:
    Texture() = default;
    ~Texture() = default;

    Texture(const Texture& other) = default;
    Texture(Texture&& other) = default;
    Texture& operator= (const Texture&) = delete;
    Texture& operator= (const Texture&&) = delete;

    void CreateTextureImage(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path);
    void Destroy();

    VkImageView& GetTextureImageView() { return m_TextureImageView; }

    static void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

private:
    VkImage m_TextureImage;
    VkDeviceMemory m_TextureImageMemory;
    VkImageView m_TextureImageView;

    void CreateTextureImageView();    
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
};

