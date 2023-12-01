#pragma once
#include "Component.h"

struct Timer : public Component {
    Timer() = default;

    float GetCountdownTime() const { return m_countdownTime; }
    void SetCountdownTime(float newTime) { m_countdownTime = newTime; }

private:
    float m_countdownTime = 60;
};
