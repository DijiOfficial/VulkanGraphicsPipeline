#pragma once
//#include <vulkan/vulkan_core.h>
#include "abstractions/DescriptorPool.h"
#include "abstractions/DataBuffer.h"
#include "textures/TextureManager.h"
#include "Utils.h"
#include <unordered_map>
#include <memory>

template<typename VertexType>
class Mesh
{
public:
    Mesh() : m_TextureManager(m_DescriptorPool) {  };
    //todo: pass by rvalue reference
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

    TextureManager& GetTextureManager() { return m_TextureManager; }

    //virtual void AddVertex(const glm::vec2& pos, const glm::vec3& color = { 1, 1, 1 }) = 0;
    void AddVertex(const VertexType& vertex);
    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
    void BindBuffers(const VkCommandBuffer& commandBuffer) const;

protected:
    bool m_IsAllocated = false;
    float m_ElapsedTime = 0.f;

    std::unique_ptr<DataBuffer> m_VertexBuffer{};
    std::unique_ptr<DataBuffer> m_IndexBuffer{};
    std::unordered_map<VertexType, uint32_t> m_VertexIndexUMap{};
    std::vector<VertexType> m_Vertices = {};
    std::vector<uint32_t> m_Indices = {};

    TextureManager m_TextureManager;
    //could be in Scene class for less overhead and memory usage but no unique textures
    DescriptorPool m_DescriptorPool;
};

#include "Mesh.inl"