#pragma once
#include "Singleton.h"

namespace diji
{
    class TimeSingleton : public Singleton<TimeSingleton>
    {
    public:
        float GetDeltaTime() const { return m_DeltaTime; }
        void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }

    private:
        float m_DeltaTime = 0.0f;
    };
}