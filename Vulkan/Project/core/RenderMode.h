#pragma once

class RenderOptions final
{
public:
    RenderOptions() = default;
    ~RenderOptions() noexcept = default;

    RenderOptions(const RenderOptions& other) = delete;
    RenderOptions(RenderOptions&& other) = delete;
    RenderOptions& operator=(const RenderOptions& other) = delete;
    RenderOptions& operator=(RenderOptions&& other) = delete;

    enum class RenderMode
    {
        Lambert,
        Normal,
        Specular,
        Combined,
    };

    void Update();
    RenderMode& GetRenderMode() { return m_RenderMode; };

private:
    RenderMode m_RenderMode = RenderMode::Combined;
    void CycleRenderMode();
};