#include "MeshlLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "Mesh.h"
#include <stdexcept>
#include <glm/ext/scalar_constants.hpp>

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
            const glm::vec2 diffX = glm::vec2(vertex1.m_TexCoord.x - vertex0.m_TexCoord.x, vertex2.m_TexCoord.x - vertex0.m_TexCoord.x);
            const glm::vec2 diffY = glm::vec2(vertex1.m_TexCoord.y - vertex0.m_TexCoord.y, vertex2.m_TexCoord.y - vertex0.m_TexCoord.y);
            float r = 1.0f / (diffX.x * diffY.y - diffY.x * diffX.y);

            glm::vec3 tangent = (deltaPos1 * diffY.y - deltaPos2 * diffY.x) * r;
            vertex0.m_Tangent += glm::normalize(Reject(tangent, vertex0.m_Normal));
            vertex1.m_Tangent += glm::normalize(Reject(tangent, vertex1.m_Normal));
            vertex2.m_Tangent += glm::normalize(Reject(tangent, vertex2.m_Normal));

            mesh->AddVertex(vertex0);
            mesh->AddVertex(vertex1);
            mesh->AddVertex(vertex2);
        }
    }    
}

void MeshLoader::InitializeSphere(Mesh<Vertex3D>* mesh, const glm::vec3& center, float radius, int sectorCount, int stackCount)
{
    const auto PI = glm::pi<float>();
    const float sectorStep = 2 * PI / sectorCount;
    const float stackStep = PI / stackCount;
    std::vector<Vertex3D> Vertices;
    for (int i = 0; i <= stackCount; ++i)
    {
        const float stackAngle = PI / 2 - i * stackStep;
        const float xy = radius * cosf(stackAngle);
        const float z = radius * sinf(stackAngle);
        for (int j = 0; j <= sectorCount; ++j)
        {
            const float sectorAngle = j * sectorStep;
            const float x = xy * cosf(sectorAngle);
            const float y = xy * sinf(sectorAngle);
            Vertex3D vertex{};
            vertex.m_Pos = { x , y , z };
            vertex.m_Normal = glm::normalize(vertex.m_Pos);
            vertex.m_Color = { 1.0f, 1.0f, 1.0f };
            Vertices.push_back(vertex);
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
                mesh->AddVertex(Vertices[k1]);
                mesh->AddVertex(Vertices[k1 + 1]);
                mesh->AddVertex(Vertices[k2]);
            }
            if (i != (stackCount - 1))
            {
                mesh->AddVertex(Vertices[k1 + 1]);
                mesh->AddVertex(Vertices[k2 + 1]);
                mesh->AddVertex(Vertices[k2]);
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
