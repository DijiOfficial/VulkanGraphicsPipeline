#include "RenderMode.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>

void RenderOptions::Update()
{
    const auto& releasedKeys = InputManager::GetInstance().GetReleasedKeys();

    if (std::find(releasedKeys.begin(), releasedKeys.end(), GLFW_KEY_Q) != releasedKeys.end())
    {
        CycleRenderMode();
    }
}

void RenderOptions::CycleRenderMode()
{
    switch (m_RenderMode)
    {
    case RenderMode::Lambert:
        m_RenderMode = RenderMode::Normal;
        break;
    case RenderMode::Normal:
        m_RenderMode = RenderMode::Specular;
        break;
    case RenderMode::Specular:
        m_RenderMode = RenderMode::Combined;
        break;
    case RenderMode::Combined:
        m_RenderMode = RenderMode::Lambert;
        break;
    default:
        break;
    }
};