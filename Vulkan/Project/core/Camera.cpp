#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "TimeSingleton.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>

Camera::Camera(const glm::vec3& _origin, float _fovAngle)
    : m_Origin{ _origin }
    , m_FOVAngle{ _fovAngle }
{
    CalculateFOV();
}

void Camera::Initialize(float _fovAngle, glm::vec3 _origin, float nearPlane, float farPLane)
{
    m_FOVAngle = _fovAngle;
    CalculateFOV();

    m_Origin = _origin;
    m_NearPlane = nearPlane;
    m_FarPlane = farPLane;
    CalculateProjectionMatrix();
}

void Camera::Update()
{
    const auto& input = InputManager::GetInstance().GetPressedKeys();
    const float deltaTime = diji::TimeSingleton::GetInstance().GetDeltaTime();
    int speed{ 2 };

    if (input.count(GLFW_KEY_LEFT_SHIFT))
        speed *= 3;

    const float translation = deltaTime * speed;

    if (input.count(GLFW_KEY_W) || input.count(GLFW_KEY_UP))
        m_Origin += m_Forward * translation;

    if (input.count(GLFW_KEY_A) || input.count(GLFW_KEY_LEFT))
        m_Origin -= m_Right * translation;

    if (input.count(GLFW_KEY_S) || input.count(GLFW_KEY_DOWN))
        m_Origin -= m_Forward * translation;

    if (input.count(GLFW_KEY_D) || input.count(GLFW_KEY_RIGHT))
        m_Origin += m_Right * translation;

    const auto& mouse = InputManager::GetInstance().GetMouseMouvementVec();

    if (InputManager::GetInstance().IsRightMousePressed() && !InputManager::GetInstance().IsMouseIdle())
    {
        if (InputManager::GetInstance().IsLeftMousePressed() && mouse.y)
                m_Origin -= m_Up * translation * mouse.y;
        else
        {
            if (mouse.x)
                m_TotalYaw += mouse.x * speed * translation;
            if (mouse.y)
                m_TotalPitch += mouse.y * speed * translation;
        }
    }

    const glm::mat4x4 rotation{ glm::rotate(glm::mat4(1.0f), glm::radians(m_TotalYaw), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(m_TotalPitch), glm::vec3(1.0f, 0.0f, 0.0f)) };
    m_Forward = rotation * glm::vec4{ FORWARD, 1.f };

    CalculateViewMatrix();
}

void Camera::CalculateViewMatrix()
{
    m_Right = glm::normalize(glm::cross(UP, m_Forward));
    m_Up = glm::normalize(glm::cross(m_Forward, m_Right));

    m_ViewMatrix = {
        glm::vec4{ m_Right, 0 },
        glm::vec4{ m_Up, 0 },
        glm::vec4{ m_Forward, 0 },
        glm::vec4{ m_Origin, 1 },
    };

    m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
}

void Camera::CalculateProjectionMatrix()
{
    m_ProjectionMatrix = glm::perspectiveLH(m_FOV, m_AspectRatio, m_NearPlane, m_FarPlane);
    m_ProjectionMatrix[1][1] *= -1;
}