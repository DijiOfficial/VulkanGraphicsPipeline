#include "DescriptorPool.h"
#include "vulkanbase/VulkanBase.h"
#include "textures/Textures.h"
#include "core/ResourceManager.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void DescriptorPool::Initialize(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout)
{
    CreateUniformBuffers(commandPool, graphicsQueue);
    CreateDescriptorPool();
	CreateDescriptorSets(descriptorSetLayout);
}

void DescriptorPool::Destroy()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_UniformBuffersVec[i]->Destroy();
    }
    m_UniformBuffersVec.clear();
    m_UniformBuffersMapped.clear();

    vkDestroyDescriptorPool(VulkanBase::device, m_DescriptorPool, nullptr);
}

void DescriptorPool::UpdateUniformBuffer(uint32_t currentFrame, UniformBufferObject ubo)
{
    memcpy(m_UniformBuffersMapped[currentFrame], &ubo, sizeof(ubo));
}

void DescriptorPool::CreateDescriptorPool()
{
    std::array<VkDescriptorPoolSize, 3> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	if (vkCreateDescriptorPool(VulkanBase::device, &poolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void DescriptorPool::CreateDescriptorSets(const VkDescriptorSetLayout& descriptorSetLayout)
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = m_DescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    m_DescriptorSetsVec.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(VulkanBase::device, &allocInfo, m_DescriptorSetsVec.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        ConfigureDescriptors(i);
    }
}

void DescriptorPool::ConfigureDescriptors(size_t idx)
{
    const VkSampler& textureSampler = ResourceManager::GetInstance().GetTextureSampler();

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = m_UniformBuffersVec[idx]->GetDataBuffer();
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = m_AlbedoImageView;
    imageInfo.sampler = textureSampler;

    VkDescriptorImageInfo normalMapInfo{};
    normalMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    normalMapInfo.imageView = m_NormalImageView;
    normalMapInfo.sampler = textureSampler;

    std::array<VkWriteDescriptorSet, 3> descriptorWrites{};

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = m_DescriptorSetsVec[idx];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = m_DescriptorSetsVec[idx];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pImageInfo = &imageInfo;

    descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[2].dstSet = m_DescriptorSetsVec[idx];
    descriptorWrites[2].dstBinding = 2;
    descriptorWrites[2].dstArrayElement = 0;
    descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[2].descriptorCount = 1;
    descriptorWrites[2].pImageInfo = &normalMapInfo;

    vkUpdateDescriptorSets(VulkanBase::device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = m_DescriptorSetsVec[idx];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr; // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional
    vkUpdateDescriptorSets(VulkanBase::device, 1, &descriptorWrite, 0, nullptr);

}

void DescriptorPool::CreateUniformBuffers(const VkCommandPool& commandPool, const VkQueue& graphicsQueue)
{
    const VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    const VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    m_UniformBuffersVec.resize(MAX_FRAMES_IN_FLIGHT);
    m_UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        m_UniformBuffersVec[i] = std::make_unique<DataBuffer>();

        DataBuffer::CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBuffersVec[i]->GetDataBuffer(), m_UniformBuffersVec[i]->GetDataBufferMemory());

        vkMapMemory(VulkanBase::device, m_UniformBuffersVec[i]->GetDataBufferMemory(), 0, bufferSize, 0, &m_UniformBuffersMapped[i]);
    }
}
