#pragma once
#include "Singleton.h"
//#include "mesh/Mesh.h"
#include "mesh/2DMesh.h"

class Scene final : public Singleton<Scene>
{
public:
    void Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, float aspectRatio);
	void Destroy();
    //temp
    void Add2DDescriptorSetLayout(const VkDescriptorSetLayout& descriptorSetLayout) { m_2DDescriptorSetLayout = descriptorSetLayout; };

    void DrawTextureMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;
    void Draw2DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;

    void CreateCircle(const glm::vec2& center, float r1, float r2, int nrOfSegments);

    void CreateRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });

    void CreateRoundedRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, float radius, int nrOfSegments);
    void CreateRoundedRectangle(float left, float bottom, float width, float height, float radius, int nrOfSegments);

    void Update(uint32_t currentFrame);

private:
    void CreateRectangle(Mesh* mesh, float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    Mesh2D* AddMesh();
    std::vector<std::unique_ptr<Mesh>> m_Meshes;
    std::vector<std::unique_ptr<Mesh2D>> m_2DMeshes;
    //std::vector<std::unique_ptr<Mesh3D>> m_3DMeshes;

    VkCommandPool m_CommandPool{};
    VkQueue m_GraphicsQueue{};
    VkDescriptorSetLayout m_TextureDescriptorSetLayout{};
    VkDescriptorSetLayout m_2DDescriptorSetLayout{};
    UniformBufferObject m_Ubo{};
};