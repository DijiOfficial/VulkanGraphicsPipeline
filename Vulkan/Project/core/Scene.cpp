#include "Scene.h"
#include <glm/gtc/constants.hpp>
#include "textures/TextureManager.h"
#include "lib/json.hpp"
#include <fstream>

void Scene::Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const glm::mat4& projectionMatrix, float aspectRatio)
{
    m_3DUniformBufferObject.proj = projectionMatrix;

    m_2DUniformBufferObject.model = glm::mat4(1.f);
    //m_2DUniformBufferObject.view = glm::mat4(1.f);
    //m_2DUniformBufferObject.proj = glm::mat4(1.f);
    m_2DUniformBufferObject.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    m_2DUniformBufferObject.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    m_2DUniformBufferObject.proj[1][1] *= -1;

    m_GraphicsQueue = graphicsQueue;
    m_CommandPool = commandPool;
    m_DescriptorSetLayout = descriptorSetLayout;
	//LevelParser::ParseLevel(m_3DMeshes, m_2DMeshes, commandPool, "resources/level.json");
    //for (auto& mesh : m_2DMeshes)
    //{
    //    mesh->MapBuffers();
    //    mesh->UploadMesh(commandPool, VulkanBase::graphicsQueue);
    //}
    //CreateRectangle({-1.f, -1.f} , 2 , 2);

    const std::vector<Vertex3D> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4
    };

    const std::vector<Vertex2D> vertices2 = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
    const std::vector<uint32_t> indices2 = { 0, 1, 2, 2, 3, 0 };

    //m_Meshes2D.push_back(std::make_unique<Mesh<Vertex2D>>(commandPool, graphicsQueue, descriptorSetLayout, vertices2, indices2));
    //m_Meshes3D.push_back(std::make_unique<Mesh<Vertex3D>>(commandPool, graphicsQueue, m_3DDescriptorSetLayout, vertices, indices));

    //CreateCircle({0.0f, 0.0f}, 0.5f, 0.5f, 32);
    //CreateRoundedRectangle(-0.5f,-0.5f, 1.0f, 1.0f, 0.1f, 8);

    //const auto& mesh = AddMesh<Vertex3D>();
    //m_MeshLoader.LoadModel(mesh, "resources/obj/vehicle.obj", true);
    //mesh->GetTextureManager().UploadAlbedoTexture(graphicsQueue, commandPool, "vehicle_diffuse.png");
    //mesh->GetTextureManager().UploadNormalTexture(graphicsQueue, commandPool, "vehicle_normal.png");
    //mesh->SetHasNormalMap();
    //mesh->GetTextureManager().UploadGlossTexture(graphicsQueue, commandPool, "vehicle_gloss.png");
    //mesh->GetTextureManager().UploadSpecularTexture(graphicsQueue, commandPool, "vehicle_specular.png");

    //mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_3DDescriptorSetLayout);

    //const auto& cube = AddMesh<Vertex3D>();
    //m_MeshLoader.LoadCube(cube, glm::vec3{ 3,12,5 }, 12);
    //cube->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_3DDescriptorSetLayout);

    LoadScene();    
}

void Scene::Destroy()
{
	for (auto& mesh : m_Meshes2D)
	{
		mesh->Destroy();
	}
    m_Meshes2D.clear();

    for (auto& mesh : m_Meshes3D)
    {
        mesh->Destroy();
    }
    m_Meshes3D.clear();
}

void Scene::Update(const glm::mat4& viewMatrix, uint32_t currentFrame)
{
    m_3DUniformBufferObject.view = viewMatrix;

    for (auto& mesh : m_Meshes2D)
    {
        mesh->Update(currentFrame, m_2DUniformBufferObject);
    }

    for (auto& mesh : m_Meshes3D)
    {
        mesh->Update(currentFrame, m_3DUniformBufferObject);
    }
}

void Scene::Draw2DMeshes(VkCommandBuffer const& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const
{
    for (const auto& mesh : m_Meshes2D)
    {
        mesh->Draw(commandBuffer, pipelineLayout, currentFrame);
    }
}

void Scene::Draw3DMeshes(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const
{
	for (const auto& mesh : m_Meshes3D)
	{
		mesh->Draw(commandBuffer, pipelineLayout, currentFrame);
	}
}

void Scene::LoadScene()
{
    std::ifstream file("resources/meshes.json");
    nlohmann::json data = nlohmann::json::parse(file);

    for (const auto& meshData : data["data"])
    {
        std::string type = meshData["type"];
        std::string shape = meshData["shape"];
        if (type == "2D")
        {
            continue;
			//auto& mesh2D = AddMesh<Vertex2D>();
   //         for (const auto& vertex : mesh["vertices"])
   //         {
			//	mesh2D.AddVertex(Vertex2D{ { vertex["position"][0], vertex["position"][1] }, { vertex["color"][0], vertex["color"][1], vertex["color"][2] } });
			//}
			//mesh2D.AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_DescriptorSetLayout);
		}
        else if (type == "3D")
        {
			const auto& mesh = AddMesh<Vertex3D>();
            if (shape == "cube")
            {
                m_MeshLoader.LoadCube(mesh, glm::vec3{ meshData["position"][0], meshData["position"][1], meshData["position"][2]}, meshData["size"]);
            }
            else if (shape == "sphere")
            {
                const int sectorCount =  meshData.contains("sectorCount") ? meshData["sectorCount"] : 36;
                const int stackCount =   meshData.contains("stackCount")  ? meshData["stackCount"]  : 18;
                
                m_MeshLoader.LoadSphere(mesh, glm::vec3{ meshData["position"][0], meshData["position"][1], meshData["position"][2] }, meshData["radius"], sectorCount, stackCount);
            }
            else if (shape == "object")
            {
                m_MeshLoader.LoadModel(mesh, meshData["path"], true);
                if (meshData.contains("textures"))
                {
                    if (meshData["textures"].contains("albedo"))
                        mesh->GetTextureManager().UploadAlbedoTexture(m_GraphicsQueue, m_CommandPool, meshData["textures"]["albedo"]);
                    if (meshData["textures"].contains("normal"))
                    {
                        mesh->GetTextureManager().UploadNormalTexture(m_GraphicsQueue, m_CommandPool, meshData["textures"]["normal"]);
                        mesh->SetHasNormalMap();
                    }
                    if (meshData["textures"].contains("gloss"))
                        mesh->GetTextureManager().UploadGlossTexture(m_GraphicsQueue, m_CommandPool, meshData["textures"]["gloss"]);
                    if (meshData["textures"].contains("specular"))
                        mesh->GetTextureManager().UploadSpecularTexture(m_GraphicsQueue, m_CommandPool, meshData["textures"]["specular"]);
                }
            }
            mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_3DDescriptorSetLayout);
		}
	}
}