#pragma once
#include "Component.h"
#include <vector>

class Animation : public Component {
public:
    int currentAnimation;
    float animationSpeed;
    std::vector<int> frames;
    float cooldownTimer;

    Animation(int anim = 0, float speed = 0.0f, const std::vector<int>& frms = {})
        : currentAnimation(anim), animationSpeed(speed), frames(frms), cooldownTimer(0.0f) {}
};