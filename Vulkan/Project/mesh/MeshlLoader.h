#pragma once
#include <glm/detail/type_vec3.hpp>
#include <string>

//temp
#include <vector>
#include "Utils.h"

#include "tiny_obj_loader.h"


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
    //3D Mesh
    void LoadModel(Mesh<Vertex3D>* mesh, const std::string& path, bool triangulate);
    //static void InitializeCube(Mesh3D& mesh, const glm::vec3& bottomLeftBackCorner, float sideLength);
    void InitializeSphere(Mesh<Vertex3D>* mesh, const glm::vec3& center, float radius);

    //2D Mesh
    //static void InitializeCircle(Mesh2D& mesh, const glm::vec2& center, float radius, int nrOfSegments);
    //static void InitializeRect(Mesh2D& mesh, const glm::vec2& bottomLeft, float sideLen);
    //static void InitializeRoundedRect(Mesh2D& mesh, float left, float bottom, float right, float top, float radius, int nrOfSegments);

    //static void LoadModelNoTan(Mesh3D& mesh, const std::string& path, bool triangulate);

private:
    Vertex3D CreateVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index);
    //static void AddRectPlane(Mesh3D& mesh, Vertex3D& bottomLeft, Vertex3D& topLeft, Vertex3D& topRight,
    //    Vertex3D& bottomRight, bool isClockWise, bool keepNormals = false);

    //static void AddRect(Mesh2D& mesh, float top, float left, float bottom, float right);
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    glm::vec3 MeshLoader::Reject(const glm::vec3& a, const glm::vec3& b)
    {
        const glm::vec3 proj = glm::dot(a, b) / glm::dot(b, b) * b;
        return a - proj;
    }
};