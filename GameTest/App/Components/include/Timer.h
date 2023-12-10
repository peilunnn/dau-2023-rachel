#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Timer : public Component {
public:
    Timer(TimerType type, float duration) : m_type(type), m_duration(duration) {}

    float GetDuration() const { return m_duration; }
    TimerType GetType() const { return m_type; }
    void SetDuration(float newDuration) { m_duration = newDuration; }

private:
    float m_duration = 0.0f;
    TimerType m_type = TimerType::None;
};
