#pragma once
#include "Component.h"
#include <vector>
using namespace std;

class Animation : public Component {
public:
    Animation() = default;

    int GetCurrentAnimation() const { return m_currentAnimation; }
    float GetCooldownTimer() const { return m_cooldownTimer; }

    void SetCurrentAnimation(int newAnimation) { m_currentAnimation = newAnimation; }
    void SetCooldownTimer(float newTime) { m_cooldownTimer = newTime; }

private:
    int m_currentAnimation = 0;
    float m_cooldownTimer = 0.0f;
    float m_animationSpeed = 0.0f;
    vector<int> m_frames = {};
};