#pragma once
#include <vulkan/vulkan_core.h>
//#include <string>
#include <memory>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "DescriptorPool.h"
//#include "DataBuffer.h"
#include "Utils.h"
#include "abstractions/DataBuffer.h"
#include <unordered_map>
#include "abstractions/DescriptorPool.h"

template<typename VertexType>
class Mesh
{
public:
    Mesh() = default;
    //pass by rvalue reference
    explicit Mesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<VertexType>& vertices, const std::vector<uint32_t>& indices)
        : m_Vertices(vertices)
        , m_Indices(indices)
    {
        AllocateBuffer(commandPool, graphicsQueue, descriptorSetLayout);
    }

    virtual ~Mesh() = default;
    void Destroy();

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = delete;

    void Update(uint32_t currentFrame, UniformBufferObject ubo);
    void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    //virtual void AddVertex(const glm::vec2& pos, const glm::vec3& color = { 1, 1, 1 }) = 0;
    void AddVertex(const VertexType& vertex);
    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
    void BindBuffers(const VkCommandBuffer& commandBuffer) const;

protected:
    bool m_IsAllocated = false;
    std::unique_ptr<DataBuffer> m_VertexBuffer{};
    std::unique_ptr<DataBuffer> m_IndexBuffer{};
    std::unordered_map<VertexType, uint32_t> m_VertexIndexUMap{};
    std::vector<VertexType> m_Vertices = {};
    std::vector<uint32_t> m_Indices = {};

    //could be in Scene class for less overhead and memory usage
    DescriptorPool m_DescriptorPool;
};

//class Mesh2D final : public Mesh<Vertex2D>
//{
//public:
//    Mesh2D() = default;
//    explicit Mesh2D(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<Vertex3D>& vertices);
//
//    ~Mesh2D() = default;
//    void Destroy();
//
//    Mesh2D(const Mesh2D& other) = delete;
//    Mesh2D(Mesh2D&& other) = delete;
//    Mesh2D& operator=(const Mesh2D& other) = delete;
//    Mesh2D& operator=(Mesh2D&& other) = delete;
//
//    void Update(uint32_t currentFrame, UniformBufferObject ubo);
//    void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;
//
//    void AddVertex(const glm::vec3& pos, const glm::vec3& color = { 1, 1, 1 });
//    void AddVertex(const Vertex3D& vertex);
//    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
//    void BindBuffers(const VkCommandBuffer& commandBuffer) const;
//
//private:
//    bool m_IsAllocated = false;
//    std::unique_ptr<DataBuffer> m_VertexBuffer{};
//    std::unique_ptr<DataBuffer> m_IndexBuffer{};
//    std::unordered_map<Vertex3D, uint32_t> m_VertexIndexUMap{};
//    std::vector<Vertex3D> m_Vertices = {};
//    std::vector<uint32_t> m_Indices = {};
//
//    //could be in Scene class for less overhead and memory usage
//    DescriptorPool m_DescriptorPool;
//};

//class Mesh2D final
//{
//public:
//    Mesh2D() = default;
//    explicit Mesh2D(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<Vertex3D>& vertices);
//
//    ~Mesh2D() = default;
//    void Destroy();
//
//    Mesh2D(const Mesh2D& other) = delete;
//    Mesh2D(Mesh2D&& other) = delete;
//    Mesh2D& operator=(const Mesh2D& other) = delete;
//    Mesh2D& operator=(Mesh2D&& other) = delete;
//
//    void Update(uint32_t currentFrame, UniformBufferObject ubo);
//    void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;
//
//    void AddVertex(const glm::vec3& pos, const glm::vec3& color = { 1, 1, 1 });
//    void AddVertex(const Vertex3D& vertex);
//    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
//    void BindBuffers(const VkCommandBuffer& commandBuffer) const;
//
//private:
//    bool m_IsAllocated = false;
//    std::unique_ptr<DataBuffer> m_VertexBuffer{};
//    std::unique_ptr<DataBuffer> m_IndexBuffer{};
//    std::unordered_map<Vertex3D, uint32_t> m_VertexIndexUMap{};
//    std::vector<Vertex3D> m_Vertices = {};
//    std::vector<uint32_t> m_Indices = {};
//
//    //could be in Scene class for less overhead and memory usage
//    DescriptorPool m_DescriptorPool;
//};

#include "Mesh.inl"