#pragma once
#include "mesh/Mesh.h"
#include "mesh/MeshlLoader.h"

class Scene final
{
public:
    Scene() = default;
    ~Scene() = default;

    Scene(const Scene& other) = delete;
    Scene(Scene&& other) = delete;
    Scene& operator=(const Scene& other) = delete;
    Scene& operator=(Scene&& other) = delete;

    void Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const glm::mat4& projectionMatrix, float aspectRatio);
	void Destroy();

    void Update(const glm::mat4& viewMatrix, uint32_t currentFrame);
    void Add3DDescriptorLayout(const VkDescriptorSetLayout& descriptorSetLayout) { m_3DDescriptorSetLayout = descriptorSetLayout; };

    void Draw2DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;
    void Draw3DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const;
private:
    std::vector<std::unique_ptr<Mesh<Vertex2D>>> m_Meshes2D;
    std::vector<std::unique_ptr<Mesh<Vertex3D>>> m_Meshes3D;

    VkCommandPool m_CommandPool{};
    VkQueue m_GraphicsQueue{};
    VkDescriptorSetLayout m_DescriptorSetLayout{};
    VkDescriptorSetLayout m_3DDescriptorSetLayout{};
    UniformBufferObject m_2DUniformBufferObject{};
    UniformBufferObject m_3DUniformBufferObject{};
    MeshLoader m_MeshLoader{};

    void LoadScene();

    template <typename Vertex>
    Mesh<Vertex>* AddMesh()
    {
        if constexpr (std::is_same<Vertex, Vertex3D>::value)
        {
            m_Meshes3D.push_back(std::make_unique<Mesh<Vertex3D>>());
            return m_Meshes3D.back().get();
        }
        else
        {
            m_Meshes2D.push_back(std::make_unique<Mesh<Vertex2D>>());
            return m_Meshes2D.back().get();
        }
    }
};