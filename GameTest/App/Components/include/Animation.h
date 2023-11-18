#ifndef ANIMATION_H
#define ANIMATION_H

#include "Component.h"
#include <vector>

class Animation : public Component {
public:
    int currentAnimation;
    float animationSpeed;
    std::vector<int> frames;

    Animation(int anim = 0, float speed = 0.0f, const std::vector<int>& frms = {})
        : currentAnimation(anim), animationSpeed(speed), frames(frms) {}
};

#endif // ANIMATION_H