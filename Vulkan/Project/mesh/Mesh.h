#pragma once
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
    explicit Mesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, std::vector<VertexType>&& vertices, std::vector<uint32_t>&& indices)
        : m_Vertices{ std::move(vertices) }
        , m_Indices{ std::move(indices) }
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

    void AddVertex(const VertexType& vertex);
    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout);
    void BindBuffers(const VkCommandBuffer& commandBuffer) const;
    void SetHasNormalMap() { m_HasNormalMap = 1; }

protected:
    bool m_IsAllocated = false;
    int m_HasNormalMap = 0;
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