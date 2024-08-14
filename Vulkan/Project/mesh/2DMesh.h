#include "Mesh.h"

class Mesh2D final : public Mesh
{
public:
    Mesh2D() = default;
    explicit Mesh2D(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<Vertex2D>& vertices);

    ~Mesh2D() override = default;
    void Destroy() override;

    Mesh2D(const Mesh2D& other) = delete;
    Mesh2D(Mesh2D&& other) = delete;
    Mesh2D& operator=(const Mesh2D& other) = delete;
    Mesh2D& operator=(Mesh2D&& other) = delete;

    //void Update(uint32_t currentFrame, UniformBufferObject ubo);
    //void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    void AddVertex(const glm::vec2& pos, const glm::vec3& color = { 1, 1, 1 });
    void AddVertex(const Vertex2D& vertex);
    void AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout) override;
    //void BindBuffers(const VkCommandBuffer& commandBuffer) const override;

private:
    //bool m_IsAllocated = false;
    //std::unique_ptr<DataBuffer> m_VertexBuffer{};
    //std::unique_ptr<DataBuffer> m_IndexBuffer{};
    std::unordered_map<Vertex2D, uint32_t> m_VertexIndexUMap{};
    std::vector<Vertex2D> m_Vertices2D = {};
    //std::vector<uint32_t> m_Indices = {};

    //could be in Scene class for less overhead and memory usage
    //DescriptorPool m_DescriptorPool;
};