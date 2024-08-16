#include "ResourceManager.h"
#include "vulkanbase/VulkanBase.h"

void ResourceManager::Init(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& data)
{
    m_DataPath = data;
    m_DefaultTexture = LoadTexture(graphicsQueue, commandPool, "default.jpg");
    CreateTextureSampler();
}

void ResourceManager::Destroy()
{
    for (auto& texture : m_TexturesUPtrUMap)
    {
        texture.second->Destroy();
    }
    m_TexturesUPtrUMap.clear();
    vkDestroySampler(VulkanBase::device, m_TextureSampler, nullptr);
}

Texture* ResourceManager::LoadTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& file)
{
    const auto fullPath = m_DataPath + file;
    const auto it = m_TexturesUPtrUMap.find(fullPath);
    if (it != m_TexturesUPtrUMap.cend())
    {
        return it->second.get();
    }

    Texture* texture = AddTexture(fullPath);
    texture->CreateTextureImage(graphicsQueue, commandPool, fullPath);
    return texture;
}

Texture* ResourceManager::AddTexture(const std::string& fullPath)
{
    m_TexturesUPtrUMap[fullPath] = std::make_unique<Texture>();
    return m_TexturesUPtrUMap[fullPath].get();
}

void ResourceManager::CreateTextureSampler()
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(VulkanBase::physicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    if (vkCreateSampler(VulkanBase::device, &samplerInfo, nullptr, &m_TextureSampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }
}
