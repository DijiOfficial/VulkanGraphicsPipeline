#pragma once
#include "textures/Textures.h"

class DescriptorPool;
class TextureManager final
{
public:
    TextureManager() = delete;
    explicit TextureManager(DescriptorPool& descriptorPool);
    ~TextureManager() = default;

    TextureManager(const TextureManager& other) = delete;
    TextureManager(TextureManager&& other) = delete;
    TextureManager& operator=(const TextureManager& other) = delete;
    TextureManager& operator=(TextureManager&& other) = delete;

    //int m_HasNormalMap = 0;

    void UploadAlbedoTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path);
    void UploadNormalTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path);
    //void UploadGlossTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path);
    //void UploadSpecularTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path);

private:
    Texture* m_AlbedoTexture{};
    Texture* m_NormalTexture{};
    Texture* m_GlossTexture{};
    Texture* m_SpecularTexture{};

    DescriptorPool& m_DescriptorPoolCopy;
};