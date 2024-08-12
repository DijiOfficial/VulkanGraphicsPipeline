#include "Scene.h"

void Scene::Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue)
{
    m_GraphicsQueue = graphicsQueue;
    m_CommandPool = commandPool;
	//LevelParser::ParseLevel(m_3DMeshes, m_2DMeshes, commandPool, "resources/level.json");
    //for (auto& mesh : m_2DMeshes)
    //{
    //    mesh->MapBuffers();
    //    mesh->UploadMesh(commandPool, VulkanBase::graphicsQueue);
    //}

    const std::vector<Vertex2D> vertices = 
    {
        {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    //m_Meshes.push_back(std::make_unique<Mesh>(vertices));

    CreateRectangle({-1.f, -1.f} , 2 , 2);
}

void Scene::Destroy()
{
	for (auto& mesh : m_Meshes)
	{
		mesh->Destroy();
	}
    m_Meshes.clear();
}

void Scene::DrawMeshes(const VkCommandBuffer& commandBuffer) const
{
	for (const auto& mesh : m_Meshes)
	{
		mesh->Draw(commandBuffer);
	}
}

void Scene::CreateRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, const glm::vec3& color)
{
	CreateRectangle(bottomLeft.x, bottomLeft.y, size.x, size.y, color);
}

void Scene::CreateRectangle(const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color)
{
    CreateRectangle(bottomLeft.x, bottomLeft.y, width, height, color);
}

void Scene::CreateRectangle(float left, float bottom, float width, float height, const glm::vec3& color)
{
    const auto& mesh = AddMesh();
    mesh->AddVertex(glm::vec2(left, bottom + height), { 1.0f, 0.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left, bottom), { 0.0f, 1.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left + width, bottom + height), { 0.0f, 0.0f, 1.0f });

    mesh->AddVertex(glm::vec2(left, bottom), { 0.0f, 1.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left + width, bottom), { 1.0f, 0.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left + width, bottom + height), { 0.0f, 0.0f, 1.0f });

    mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue);
}

Mesh* Scene::AddMesh()
{
    m_Meshes.push_back(std::make_unique<Mesh>());
    return m_Meshes.back().get();
}
