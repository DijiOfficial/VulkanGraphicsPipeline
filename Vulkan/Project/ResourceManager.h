#pragma once
#include "Singleton.h"
#include "textures/Textures.h"
#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager final : public Singleton<ResourceManager>
{
public:
	void Init(const std::string& data) { m_DataPath = data; };
	void Destroy();

	Texture* LoadTexture(const VkQueue& graphicsQueue, const VkCommandPool& commandPool, const std::string& file);
private:
	std::string m_DataPath;

	std::unordered_map<std::string, std::unique_ptr<Texture>> m_TexturesUPtrUMap;
	Texture* AddTexture(const std::string& fullPath);
    
};