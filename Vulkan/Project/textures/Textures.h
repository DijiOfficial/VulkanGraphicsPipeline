#pragma once
#include <vulkan/vulkan_core.h>
#include <string>

//struct ImageInfoStruct
//{
//    uint32_t width;
//    uint32_t height;
//    VkFormat format;
//    VkImageTiling tiling;
//    VkImageUsageFlags usage;
//    VkMemoryPropertyFlags properties;
//};

class Texture final
{
public:
    Texture() = default;
    ~Texture() = default;

    Texture(const Texture& other) = default;
    Texture(Texture&& other) = default;
    //Texture& operator=(const Texture& other) noexcept;
    //Texture& operator=(Texture&& other) noexcept;
    //bool operator==(const Texture& rhs) const;
    //bool operator!=(const Texture& rhs) const;

    //TODO: Add a texture manager that checks whether a texture path is already loaded
    void CreateTextureImage(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path);
    void Destroy();

    void CreateTextureSampler();
    inline static VkSampler m_TextureSampler;
    inline static VkImageView m_TextureImageView;

private:
    VkImage m_TextureImage;
    VkDeviceMemory m_TextureImageMemory;

    void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void CreateTextureImageView();    
    void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, const VkQueue& graphicsQueue, const VkCommandPool& commandPool);

};

