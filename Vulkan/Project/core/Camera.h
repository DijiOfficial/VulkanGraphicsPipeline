#pragma once
#include <glm/glm.hpp>

class Camera final
{
public:
    Camera() = default;
    Camera(const glm::vec3& _origin, float _fovAngle);
    ~Camera() = default;

    Camera(const Camera& other) = delete;
    Camera(Camera&& other) noexcept = delete;
    Camera& operator=(const Camera& other) = delete;
    Camera& operator=(Camera&& other) noexcept = delete;

    void Initialize(float _fovAngle = 90.f, glm::vec3 _origin = { 0.0f, 0.0f, 0.0f }, float nearPlane = 1.0f, float farPlane = 1000.0f);
    void Update();

    void CalculateViewMatrix();
    void CalculateProjectionMatrix();

    void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
    glm::vec3 GetPosition() const { return m_Origin; }

    glm::mat4x4 GetInverseViewMatrix() const { return m_InverseViewMatrix; }
    glm::mat4x4 GetProjectionMatrix()  const { return m_ProjectionMatrix; }

private:
    void CalculateFOV() { m_FOV = glm::radians(m_FOVAngle) * 0.5f; };

    glm::mat4x4 m_InverseViewMatrix{};
    glm::mat4x4 m_ViewMatrix{};
    glm::mat4x4 m_ProjectionMatrix{};

    glm::vec3   m_Origin = { 0.0f, 0.0f, 0.0f };
    float       m_FOVAngle = { 90.f };
    float       m_FOV{};
    float       m_AspectRatio{};
    float       m_NearPlane = { 1.f };
    float       m_FarPlane = { 1000.f };

    constexpr static glm::vec3 RIGHT{ 1.f, 0.f, 0.f };
    constexpr static glm::vec3 UP{ 0.f, 1.f, 0.f };
    constexpr static glm::vec3 FORWARD{ 0.f, 0.f, 1.f };

    glm::vec3 m_Right{ RIGHT };
    glm::vec3 m_Up{ UP };
    glm::vec3 m_Forward{ FORWARD };

    float  m_TotalPitch{};
    float  m_TotalYaw{};
};