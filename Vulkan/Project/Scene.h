#pragma once
#include "Singleton.h"
#include "mesh/Mesh.h"

class Mesh;
class Scene final : public Singleton<Scene>
{
public:
    void Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, float aspectRatio);
	void Destroy();

    void DrawMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    void CreateCircle(const glm::vec2& center, float r1, float r2, int nrOfSegments);

    void CreateRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });

    void CreateRoundedRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, float radius, int nrOfSegments);
    void CreateRoundedRectangle(float left, float bottom, float width, float height, float radius, int nrOfSegments);

    void Update(uint32_t currentFrame);

private:
    void CreateRectangle(Mesh* mesh, float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    Mesh* AddMesh();
    std::vector<std::unique_ptr<Mesh>> m_Meshes;

    VkCommandPool m_CommandPool{};
    VkQueue m_GraphicsQueue{};
    VkDescriptorSetLayout m_DescriptorSetLayout{};
    UniformBufferObject m_Ubo{};
};