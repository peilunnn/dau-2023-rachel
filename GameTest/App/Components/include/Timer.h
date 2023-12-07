#pragma once
#include "Component.h"

class Timer : public Component {
public:
    Timer() = default;

    float GetCountdownTime() const { return m_countdownTime; }
    void SetCountdownTime(float newTime) { m_countdownTime = newTime; }

private:
    float m_countdownTime = 60.0f;
};
