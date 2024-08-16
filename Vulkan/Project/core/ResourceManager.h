#pragma once
#include "Singleton.h"
#include "textures/Textures.h"
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	void Init(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& data);
	void Destroy();

	VkSampler& GetTextureSampler() { return m_TextureSampler; }

	Texture* LoadTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& file);
	Texture* GetDefaultTexture() const { return m_DefaultTexture; }
private:
	std::string m_DataPath;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_TexturesUPtrUMap;
	Texture* AddTexture(const std::string& fullPath);
	Texture* m_DefaultTexture;
	VkSampler m_TextureSampler;

	void CreateTextureSampler();
};