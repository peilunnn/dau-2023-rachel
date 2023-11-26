#pragma once
#include "Component.h"
#include <vector>

class Animation : public Component {
public:
    int currentAnimation = 0;
    float animationSpeed = 0.0f;
    std::vector<int> frames = {};
    float cooldownTimer = 0.0f;

    Animation() = default;
};