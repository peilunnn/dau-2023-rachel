#pragma once
#include "Components/include/Component.h"

class Timer : public Component {
public:
    Timer(float duration = 0.0f) : m_duration(duration) {}

    float GetDuration() const { return m_duration; }
    void SetDuration(float newDuration) { m_duration = newDuration; }

private:
    float m_duration = 0.0f;
};
