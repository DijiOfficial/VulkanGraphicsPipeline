#pragma once
#include "DataBuffer.h"
#include <vector>
#include <memory>

class VulkanShader;
struct UniformBufferObject;

class DescriptorPool final
{
public:
    DescriptorPool() = default;
    void Initialize(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
    void Destroy();
    void UpdateUniformBuffer(uint32_t currentFrame, UniformBufferObject ubo);

    DescriptorPool(const DescriptorPool& other) = delete;
    DescriptorPool(DescriptorPool&& other) = delete;
    DescriptorPool& operator=(const DescriptorPool& other) = delete;
    DescriptorPool& operator=(DescriptorPool&& other) = delete;

    const VkDescriptorSet& GetDescriptorSets(uint32_t idx) const { return m_DescriptorSetsVec[idx]; };
    void SetAlbedo(VkImageView imageView) { m_AlbedoImageView = imageView; };
    //void SetNormal(VkImageView imageView);
    //void SetGloss(VkImageView imageView);
    //void SetSpecular(VkImageView imageView);

private:
    std::vector<void*> m_UniformBuffersMapped;
    std::vector<std::unique_ptr<DataBuffer>> m_UniformBuffersVec{};
    std::vector<VkDescriptorSet> m_DescriptorSetsVec;
    VkDescriptorPool m_DescriptorPool;

    VkImageView m_AlbedoImageView;
    //VkImageView m_NormalImageView;
    //VkImageView m_GlossImageView;
    //VkImageView m_SpecularImageView;

    void CreateDescriptorPool();
    void CreateDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout);
    void ConfigureDescriptors(size_t idx);
    void CreateUniformBuffers(const VkCommandPool& commandPool, const VkQueue& graphicsQueue);
};
