#pragma once
#include <glm/detail/type_vec3.hpp>
#include "Utils.h"
#include "tiny_obj_loader.h"
#include <string>

template<typename VertexType>
class Mesh;
class MeshLoader final
{
public:
    MeshLoader() = default;
    ~MeshLoader() = default;

    MeshLoader(const MeshLoader& other) = delete;
    MeshLoader(MeshLoader&& other) = delete;
    MeshLoader& operator=(const MeshLoader& other) = delete;
    MeshLoader& operator=(MeshLoader&& other) = delete;


    void LoadModel(Mesh<Vertex3D>* mesh, const std::string& path, bool triangulate);
    void LoadCube(Mesh<Vertex3D>* mesh, const glm::vec3& center, float size);
    void LoadSphere(Mesh<Vertex3D>* mesh, const glm::vec3& center, float radius, int sectorCount = 36, int stackCount = 18);


    void CreateCircle(Mesh<Vertex2D>* mesh, const glm::vec2& center, float r1, float r2, int nrOfSegments);

    void CreateRectangle(Mesh<Vertex2D>* mesh, const glm::vec2& bottomLeft, const glm::vec2& size, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(Mesh<Vertex2D>* mesh, const glm::vec2& bottomLeft, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });
    void CreateRectangle(Mesh<Vertex2D>* mesh, float left, float bottom, float width, float height, const glm::vec3& color = { 1.f, 1.f, 1.f });

    void CreateRoundedRectangle(Mesh<Vertex2D>* mesh, const glm::vec2& bottomLeft, const glm::vec2& size, float radius, int nrOfSegments);
    void CreateRoundedRectangle(Mesh<Vertex2D>* mesh, float left, float bottom, float width, float height, float radius, int nrOfSegments);

private:
    Vertex3D CreateVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index);

    glm::vec3 MeshLoader::Reject(const glm::vec3& a, const glm::vec3& b)
    {
        const glm::vec3 proj = glm::dot(a, b) / glm::dot(b, b) * b;
        return a - proj;
    }
};