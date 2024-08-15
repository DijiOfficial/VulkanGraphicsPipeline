#include "ResourceManager.h"

void ResourceManager::Destroy()
{
    for (auto& texture : m_TexturesUPtrUMap)
    {
        texture.second->Destroy();
    }
    m_TexturesUPtrUMap.clear();
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
    texture->CreateTextureImage(graphicsQueue, commandPool, file);
    return texture;
}

Texture* ResourceManager::AddTexture(const std::string& fullPath)
{
    m_TexturesUPtrUMap[fullPath] = std::make_unique<Texture>();
    return m_TexturesUPtrUMap[fullPath].get();
}
