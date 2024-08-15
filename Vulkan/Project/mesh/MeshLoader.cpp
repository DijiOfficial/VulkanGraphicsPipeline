#include "MeshlLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Mesh.h"

void MeshLoader::LoadModel(Mesh<Vertex3D>* mesh, const std::string& path, bool triangulate)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), 0, triangulate))
    {
        throw std::runtime_error(err);
    }

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex3D vertex{};

            //vertices.push_back(vertex);
            //indices.push_back(indices.size());

            vertex.m_Pos = {
            attrib.vertices[3 * index.vertex_index + 0],
            attrib.vertices[3 * index.vertex_index + 1],
            attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.m_TexCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.m_Color = { 1.0f, 1.0f, 1.0f };

            mesh->AddVertex(vertex);
        }
    }
}

void MeshLoader::InitializeSphere(Mesh<Vertex3D>* mesh, const glm::vec3& center, float radius)
{
    //mesh.ClearIndices();
    //mesh.ClearVertices();
    //mesh->GetTransform().Translate(center);
    constexpr int sectorCount = 36;
    constexpr int stackCount = 18;
    const auto PI = glm::pi<float>();
    const float sectorStep = 2 * PI / sectorCount;
    const float stackStep = PI / stackCount;
    std::vector<Vertex3D> tempVertices;
    for (int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = PI / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        for (int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * sectorStep;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            Vertex3D vertex{};
            vertex.m_Pos = { x , y , z };
            //vertex.m_Normal = glm::normalize(vertex.m_Pos);
            vertex.m_Color = { 1.0f, 1.0f, 1.0f };
            tempVertices.push_back(vertex);
        }
    }
    for (int i = 0; i < stackCount; ++i)
    {
        int k1 = i * (sectorCount + 1);
        int k2 = k1 + sectorCount + 1;
        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                mesh->AddVertex(tempVertices[k1]);
                mesh->AddVertex(tempVertices[k1 + 1]);
                mesh->AddVertex(tempVertices[k2]);
            }
            if (i != (stackCount - 1))
            {
                mesh->AddVertex(tempVertices[k1 + 1]);
                mesh->AddVertex(tempVertices[k2 + 1]);
                mesh->AddVertex(tempVertices[k2]);
            }
        }
    }
}
