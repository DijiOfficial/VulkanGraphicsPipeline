#include "TextureManager.h"
#include "core/ResourceManager.h"
#include "abstractions/DescriptorPool.h"

TextureManager::TextureManager(DescriptorPool& descriptorPool)
    : m_DescriptorPoolCopy{ descriptorPool }
{
    const auto& defaultImageView = ResourceManager::GetInstance().GetDefaultTexture()->GetTextureImageView();
    descriptorPool.SetAlbedo(defaultImageView);
    descriptorPool.SetNormal(defaultImageView);
    descriptorPool.SetGloss(defaultImageView);
    descriptorPool.SetSpecular(defaultImageView);
}

void TextureManager::UploadAlbedoTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path)
{
    m_AlbedoTexture = ResourceManager::GetInstance().LoadTexture(graphicsQueue, commandPool, path);
    m_DescriptorPoolCopy.SetAlbedo(m_AlbedoTexture->GetTextureImageView());
}

void TextureManager::UploadNormalTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path)
{
    m_NormalTexture = ResourceManager::GetInstance().LoadTexture(graphicsQueue, commandPool, path);
	m_DescriptorPoolCopy.SetNormal(m_NormalTexture->GetTextureImageView());
}

void TextureManager::UploadGlossTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path)
{
	m_GlossTexture = ResourceManager::GetInstance().LoadTexture(graphicsQueue, commandPool, path);
	m_DescriptorPoolCopy.SetGloss(m_GlossTexture->GetTextureImageView());
}

void TextureManager::UploadSpecularTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& path)
{
	m_SpecularTexture = ResourceManager::GetInstance().LoadTexture(graphicsQueue, commandPool, path);
	m_DescriptorPoolCopy.SetSpecular(m_SpecularTexture->GetTextureImageView());
}
