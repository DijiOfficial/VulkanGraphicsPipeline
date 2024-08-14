#include "Scene.h"
#include <glm/gtc/constants.hpp>

void Scene::Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, float aspectRatio)
{
    m_Ubo.model = glm::mat4(1.f);
    m_Ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_Ubo.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    m_Ubo.proj[1][1] *= -1;

    m_GraphicsQueue = graphicsQueue;
    m_CommandPool = commandPool;
    m_TextureDescriptorSetLayout = descriptorSetLayout;
    //LevelParser::ParseLevel(m_3DMeshes, m_2DMeshes, commandPool, "resources/level.json");
    //for (auto& mesh : m_2DMeshes)
    //{
    //    mesh->MapBuffers();
    //    mesh->UploadMesh(commandPool, VulkanBase::graphicsQueue);
    //}

    //CreateRectangle({ -1.f, -1.f }, 2, 2);

    const std::vector<Vertex3D> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<TextureVertex2D> vertices2 = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
    const std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

    //m_Meshes.push_back(std::make_unique<Mesh>(vertices));
    m_Meshes.push_back(std::make_unique<Mesh>(commandPool, graphicsQueue, descriptorSetLayout, vertices2, indices));

    // this won't work unless I give it a TexCoord or give it a different shader/mesh
    //CreateRectangle({-1.f, -1.f} , 2 , 2);
    //CreateRectangle({-1.f, -1.f} , 2 , 2);

    //CreateCircle({0.0f, 0.0f}, 0.5f, 0.66f, 32);
    //CreateRoundedRectangle(-0.5f,-0.5f, 1.0f, 1.0f, 0.1f, 8);
}

void Scene::Destroy()
{
    for (auto& mesh : m_Meshes)
    {
        mesh->Destroy();
    }
    m_Meshes.clear();
}

void Scene::DrawTextureMeshes(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const
{
    for (const auto& mesh : m_Meshes)
    {
        mesh->Draw(commandBuffer, pipelineLayout, currentFrame);
    }

    //for (const auto& mesh : m_3DMeshes)
    //{
    //    mesh->Draw(commandBuffer, pipelineLayout, currentFrame);
    //}
}

void Scene::Draw2DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const
{
    for (const auto& mesh : m_2DMeshes)
    {
        mesh->Draw(commandBuffer, pipelineLayout, currentFrame);
    }
}

void Scene::CreateCircle(const glm::vec2& center, float r1, float r2, int nrOfSegments)
{
    const auto& mesh = AddMesh();
    const float step = 2.0f * glm::pi<float>() / static_cast<float>(nrOfSegments);

    for (int i = 0; i < nrOfSegments; ++i)
    {
        const float theta1 = i * step;
        const float theta2 = (i + 1) * step;

        const glm::vec2 p1 = center + glm::vec2(r1 * cos(theta1), r2 * sin(theta1));
        const glm::vec2 p2 = center + glm::vec2(r1 * cos(theta2), r2 * sin(theta2));

        mesh->AddVertex(center, { 1.0f, 1.0f, 1.0f });
        mesh->AddVertex(p1, { 1.0f, 0.0f, 0.0f });
        mesh->AddVertex(p2, { 1.0f, 0.0f, 0.0f });
    }

    mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_2DDescriptorSetLayout);
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

    mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_2DDescriptorSetLayout);
}

void Scene::CreateRectangle(Mesh* mesh, float left, float bottom, float width, float height, const glm::vec3& color)
{
    mesh->AddVertex(glm::vec2(left, bottom + height), { 1.0f, 0.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left, bottom), { 0.0f, 1.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left + width, bottom + height), { 0.0f, 0.0f, 1.0f });

    mesh->AddVertex(glm::vec2(left, bottom), { 0.0f, 1.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left + width, bottom), { 1.0f, 0.0f, 0.0f });
    mesh->AddVertex(glm::vec2(left + width, bottom + height), { 0.0f, 0.0f, 1.0f });
}

void Scene::CreateRoundedRectangle(const glm::vec2& bottomLeft, const glm::vec2& size, float radius, int nrOfSegments)
{
    CreateRoundedRectangle(bottomLeft.x, bottomLeft.y, size.x, size.y, radius, nrOfSegments);
}

void Scene::CreateRoundedRectangle(float left, float bottom, float width, float height, float radius, int nrOfSegments)
{
    const auto& mesh = AddMesh();

    CreateRectangle(mesh, left + radius, bottom, width - 2 * radius, height);
    CreateRectangle(mesh, left, bottom + radius, radius, height - 2 * radius);
    CreateRectangle(mesh, left + width - radius, bottom + radius, radius, height - 2 * radius);

    auto createQuarterCircle = [&](const glm::vec2& center, float startAngle)
        {
            const float step = glm::half_pi<float>() / static_cast<float>(nrOfSegments);

            for (int i = 0; i < nrOfSegments; ++i)
            {
                const float theta1 = startAngle + i * step;
                const float theta2 = startAngle + (i + 1) * step;

                const glm::vec2 p1 = center + glm::vec2(radius * cos(theta1), radius * sin(theta1));
                const glm::vec2 p2 = center + glm::vec2(radius * cos(theta2), radius * sin(theta2));

                mesh->AddVertex(center, { 1.0f, 0.0f, 0.0f });
                mesh->AddVertex(p1, { 0.0f, 0.0f, 1.0f });
                mesh->AddVertex(p2, { 0.0f, 0.0f, 1.0f });
            }
        };

    createQuarterCircle({ left + radius, bottom + radius }, glm::pi<float>());                          // Top-left corner
    createQuarterCircle({ left + width - radius, bottom + radius }, -glm::half_pi<float>());            // Top-right corner
    createQuarterCircle({ left + width - radius, bottom + height - radius }, 0.0f);                     // Bottom-right corner
    createQuarterCircle({ left + radius, bottom + height - radius }, glm::half_pi<float>());            // Bottom-left corner

    mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_2DDescriptorSetLayout);
}

void Scene::Update(uint32_t currentFrame)
{
    for (auto& mesh : m_Meshes)
    {
        mesh->Update(currentFrame, m_Ubo);
    }

    for (auto& mesh : m_2DMeshes)
    {
        mesh->Update(currentFrame, m_Ubo);
    }
 //   for (auto& mesh : m_3DMeshes)
 //   {
 //       mesh->Update(currentFrame, m_Ubo);
	//}
}

Mesh2D* Scene::AddMesh()
{
    m_2DMeshes.push_back(std::make_unique<Mesh2D>());
    return m_2DMeshes.back().get();
}
