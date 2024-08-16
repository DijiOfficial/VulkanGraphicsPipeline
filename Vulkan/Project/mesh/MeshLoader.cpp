#include "MeshlLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Mesh.h"
#include <stdexcept>

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
        for (size_t i = 0; i < shape.mesh.indices.size(); i += 3)
        {
            // Indices for the triangle vertices
            auto& vertex0 = CreateVertex(attrib, shape.mesh.indices[i + 0]);
            auto& vertex1 = CreateVertex(attrib, shape.mesh.indices[i + 1]);
            auto& vertex2 = CreateVertex(attrib, shape.mesh.indices[i + 2]);

            // Edges of the triangle: position delta
            const glm::vec3 deltaPos1 = vertex1.m_Pos - vertex0.m_Pos;
            const glm::vec3 deltaPos2 = vertex2.m_Pos - vertex0.m_Pos;

            // UV delta
            const glm::vec2 deltaUV1 = vertex1.m_TexCoord - vertex0.m_TexCoord;
            const glm::vec2 deltaUV2 = vertex2.m_TexCoord - vertex0.m_TexCoord;

            // Calculate the tangent
            const float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

            // Gram-Schmidt orthogonalize tangent with respect to normal
            tangent = glm::normalize(tangent - vertex0.m_Normal * glm::dot(vertex0.m_Normal, tangent));
            tangent.z *= -1;

            // Store the tangent for each vertex of the triangle
            vertex0.m_Tangent = tangent;
            vertex1.m_Tangent = tangent;
            vertex2.m_Tangent = tangent;

            mesh->AddVertex(vertex0);
            mesh->AddVertex(vertex1);
            mesh->AddVertex(vertex2);
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

Vertex3D MeshLoader::CreateVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index)
{
    Vertex3D vertex{};
    
    vertex.m_Pos = {
        attrib.vertices[3 * index.vertex_index + 0],
        attrib.vertices[3 * index.vertex_index + 1],
        -attrib.vertices[3 * index.vertex_index + 2]
    };

    vertex.m_Normal = {
		attrib.normals[3 * index.normal_index + 0],
		attrib.normals[3 * index.normal_index + 1],
		-attrib.normals[3 * index.normal_index + 2]
	};

    if (index.texcoord_index >= 0)
    {
        vertex.m_TexCoord = {
			attrib.texcoords[2 * index.texcoord_index + 0],
			1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
		};
	}

	return vertex;
}
