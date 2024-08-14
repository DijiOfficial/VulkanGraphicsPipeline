#pragma once
#include "Singleton.h"
#include "mesh/Mesh.h"

class Scene final : public Singleton<Scene>
{
public:
    void Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, float aspectRatio);
	void Destroy();

    void Add3DDescriptorLayout(const VkDescriptorSetLayout& descriptorSetLayout) { m_3DDescriptorSetLayout = descriptorSetLayout; };

    void Draw2DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;
    void Draw3DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    void CreateCircle(const glm::vec2& center, float r1, float r2, int nrOfSegments);

    void CreateRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });

    void CreateRoundedRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, float radius, int nrOfSegments);
    void CreateRoundedRectangle(float left, float bottom, float width, float height, float radius, int nrOfSegments);

    void Update(uint32_t currentFrame);

private:
    void CreateRectangle(Mesh<Vertex2D>* mesh, float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    Mesh<Vertex2D>* AddMesh();
    std::vector<std::unique_ptr<Mesh<Vertex2D>>> m_Meshes2D;
    std::vector<std::unique_ptr<Mesh<Vertex3D>>> m_Meshes3D;

    VkCommandPool m_CommandPool{};
    VkQueue m_GraphicsQueue{};
    VkDescriptorSetLayout m_DescriptorSetLayout{};
    VkDescriptorSetLayout m_3DDescriptorSetLayout{};
    UniformBufferObject m_Ubo{};
};