#pragma once
#include <vulkan/vulkan_core.h>
//#include <string>
#include <memory>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "DescriptorPool.h"
//#include "DataBuffer.h"
#include "Utils.h"
#include "DataBuffer.h"
#include <unordered_map>
#include "DescriptorPool.h"

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<Vertex2D>& vertices);
    
    virtual ~Mesh() = default;
    virtual void Destroy();

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = delete;

    virtual void Update(uint32_t currentFrame, UniformBufferObject ubo);
    virtual void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    void AddVertex(const glm::vec2& pos, const glm::vec3& color = { 1, 1, 1 });
    void AddVertex(const Vertex2D& vertex);
    virtual void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
    virtual void BindBuffers(const VkCommandBuffer& commandBuffer) const;

protected:
    bool m_IsAllocated = false;
    std::unique_ptr<DataBuffer> m_VertexBuffer{};
    std::unique_ptr<DataBuffer> m_IndexBuffer{};
    std::vector<uint32_t> m_Indices = {};

    //could be in Scene class for less overhead and memory usage
    DescriptorPool m_DescriptorPool;

private:
    std::unordered_map<Vertex2D, uint32_t> m_VertexIndexUMap{};
    std::vector<Vertex2D> m_Vertices = {};
};

class Mesh3D final : public Mesh
{
public:
    Mesh3D() = default;
    explicit Mesh3D(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<Vertex3D>& vertices);

    ~Mesh3D() override = default;

    Mesh3D(const Mesh3D& other) = delete;
    Mesh3D(Mesh3D&& other) = delete;
    Mesh3D& operator=(const Mesh3D& other) = delete;
    Mesh3D& operator=(Mesh3D&& other) = delete;

    //void Update(uint32_t currentFrame, UniformBufferObject ubo);
    //void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    void AddVertex(const glm::vec3& pos, const glm::vec3& color = { 1, 1, 1 });
    void AddVertex(const Vertex3D& vertex);
    //void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
    //void BindBuffers(const VkCommandBuffer& commandBuffer) const;

private:
    //bool m_IsAllocated = false;
    //std::unique_ptr<DataBuffer> m_VertexBuffer{};
    //std::unique_ptr<DataBuffer> m_IndexBuffer{};
    std::unordered_map<Vertex3D, uint32_t> m_VertexIndexUMap{};
    std::vector<Vertex3D> m_Vertices = {};
    //std::vector<uint32_t> m_Indices = {};

    //could be in Scene class for less overhead and memory usage
    //DescriptorPool m_DescriptorPool;
};