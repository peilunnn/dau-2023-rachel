#pragma once
#include "Components/include/Component.h"
#include <vector>
using namespace std;

class Animation : public Component {
public:
    Animation() = default;

    int GetCurrentAnimation() const { return m_currentAnimation; }
    void SetCurrentAnimation(int newAnimation) { m_currentAnimation = newAnimation; }

private:
    int m_currentAnimation = 0;
    float m_animationSpeed = 0.0f;
    vector<int> m_frames = {};
};