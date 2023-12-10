#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Timer : public Component {
public:
    Timer::Timer(TimerType type, float initialDuration)
        : m_type(type), m_initialDuration(initialDuration), m_remainingTime(initialDuration) {}

    float GetInitialDuration() const { return m_initialDuration; }
    float GetRemainingTime() const { return m_remainingTime; }
    TimerType GetType() const { return m_type; }
    void SetRemainingTime(float newRemainingTime) { m_remainingTime = newRemainingTime; }

private:
    float m_initialDuration = 0.0f;
    float m_remainingTime = 0.0f;
    TimerType m_type = TimerType::None;
};
