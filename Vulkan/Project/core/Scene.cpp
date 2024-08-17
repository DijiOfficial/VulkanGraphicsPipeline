#include "Scene.h"
#include <glm/gtc/constants.hpp>
#include "textures/TextureManager.h"
#include "lib/json.hpp"
#include <fstream>

void Scene::Init(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const glm::mat4& projectionMatrix, float aspectRatio)
{
    m_3DUniformBufferObject.proj = projectionMatrix;
    m_3DUniformBufferObject.model = glm::rotate(glm::mat4(1.0f), glm::radians(180.f), Camera::UP);

    m_2DUniformBufferObject.model = glm::mat4(1.f);
    m_2DUniformBufferObject.view = glm::mat4(1.f);
    m_2DUniformBufferObject.proj = glm::mat4(1.f);

    //m_2DUniformBufferObject.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //m_2DUniformBufferObject.proj = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    //m_2DUniformBufferObject.proj[1][1] *= -1;

    m_GraphicsQueue = graphicsQueue;
    m_CommandPool = commandPool;
    m_DescriptorSetLayout = descriptorSetLayout;
	

    //CreateRectangle({-1.f, -1.f} , 2 , 2);

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
            const auto& mesh = AddMesh<Vertex2D>();
            if (shape == "rectangle")
            {
                if (meshData.contains("texture"))
                {
                    mesh->GetTextureManager().UploadAlbedoTexture(m_GraphicsQueue, m_CommandPool, meshData["texture"]);
                    mesh->SetHasTexture();
				}
                m_MeshLoader.CreateRectangle(mesh, glm::vec2{ meshData["position"][0], meshData["position"][1] }, meshData["size"][0], meshData["size"][1]);
                mesh->SetRotationAngle(179.9f);
			}
            else if (shape == "oval")
            {
				m_MeshLoader.CreateCircle(mesh, glm::vec2{ meshData["position"][0], meshData["position"][1] }, meshData["radius"][0], meshData["radius"][1], meshData["segments"]);
                mesh->SetRotationAngle(179.9f);
            }
            else if (shape == "roundedRectangle")
            {
                m_MeshLoader.CreateRoundedRectangle(mesh, glm::vec2{ meshData["position"][0], meshData["position"][1] }, glm::vec2{ meshData["size"][0], meshData["size"][1] }, meshData["radius"], meshData["segments"]);
                mesh->SetRotationAngle(180.1f);
            }
            else
            {
                throw std::runtime_error("failed to load 2D data shape! " + shape);
            }
            mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_DescriptorSetLayout);
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

                mesh->Translate(glm::vec3{ meshData["position"][0], meshData["position"][1], meshData["position"][2] });
                
                if (meshData.contains("scale"))
                    mesh->SetScale(glm::vec3{ meshData["scale"][0], meshData["scale"][1], meshData["scale"][2] });

                if (meshData.contains("rotation"))
                {
                    mesh->Rotate({ meshData["rotation"][0], meshData["rotation"][1], meshData["rotation"][2] });
                    mesh->DisableRotation();
                }
            }
            else
            {
				throw std::runtime_error("failed to load 3D data shape! " + shape);
			}
            mesh->AllocateBuffer(m_CommandPool, m_GraphicsQueue, m_3DDescriptorSetLayout);
		}
        else
        {
			throw std::runtime_error("failed to load data type! " + type);
		}
	}
}