#pragma once
#include "Singleton.h"
#include "mesh/Mesh.h"

class Mesh;
class Scene final : public Singleton<Scene>
{
public:
    void Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue);
	void Destroy();

    void DrawMeshes(VkCommandBuffer const& commandBuffer) const;

    void CreateCircle(const glm::vec2& center, float r1, float r2, int nrOfSegments);

    void CreateRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });

    void CreateRoundedRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, float radius, int nrOfSegments);
    void CreateRoundedRectangle(float left, float bottom, float width, float height, float radius, int nrOfSegments);
    //void DrawMeshes(VkCommandBuffer const& commandBuffer, uint32_t currentFrame) const;

//    static int m_AreNormalsEnabled;
//    static DescriptorPool& Get2DDescriptorPool() { return m_2DDescriptorPool; }
//    void Update(uint32_t currentFrame, const glm::mat4& viewMatrix);
//    void Draw3DMeshes(const VkCommandBuffer& commandBuffer, uint32_t currentFrame) const;
//
//    void WindowHasBeenResized(const glm::mat4& projMatrix);
//    void Init(const VkCommandPool& commandPool, const glm::mat4& projMatrix);

private:
    void CreateRectangle(Mesh* mesh, float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
//    std::vector<std::unique_ptr<Mesh3D>> m_3DMeshes;
    Mesh* AddMesh();
    std::vector<std::unique_ptr<Mesh>> m_Meshes;

    VkCommandPool m_CommandPool{};
    VkQueue m_GraphicsQueue{};
//    UniformBufferObject m_3DUBOMatrixes{};
//    UniformBufferObject m_2DUBOMatrixes{};
//
//    static DescriptorPool m_2DDescriptorPool;
};