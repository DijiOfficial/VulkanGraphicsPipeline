#pragma once
#include <vulkan/vulkan_core.h>
//#include <string>
#include <memory>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include "DescriptorPool.h"
//#include "DataBuffer.h"
#include "Vertex.h"
#include "DataBuffer.h"
#include <unordered_map>

class Mesh
{
public:
    Mesh() = default;
    explicit Mesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const std::vector<Vertex2D>& vertices);
    
    //explicit Mesh(std::vector<Vertex2D>&& vertices, std::vector<uint32_t>&& indices);
    ~Mesh() = default;
    void Destroy();

    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = delete;

    //void Update(uint32_t currentFrame);
    void Draw(const VkCommandBuffer& commandBuffer) const;
    //void Draw(const VkCommandBuffer& commandBuffer, uint32_t currentFrame) const;

    void ClearVertices() { m_Vertices.clear(); };
    //void ClearIndices() { m_Indices.clear(); };
    //void UploadMesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue);

    void AddVertex(const glm::vec2& pos, const glm::vec3& color = { 1, 1, 1 });
    void AddVertex(const Vertex2D& vertex);
    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue);
    void BindBuffers(const VkCommandBuffer& commandBuffer) const;

private:
    bool m_IsAllocated = false;
    std::unique_ptr<DataBuffer> m_VertexBuffer{};
    std::unique_ptr<DataBuffer> m_IndexBuffer{};
    std::unordered_map<Vertex2D, uint32_t> m_VertexIndexUMap{};
    std::vector<Vertex2D> m_Vertices = {};
    std::vector<uint32_t> m_Indices = {};
};