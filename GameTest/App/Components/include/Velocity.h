#pragma once
#include <glm/vec2.hpp>
#include "Component.h"

class Velocity : public Component {
public:
    glm::vec2 velocity;

    Velocity(float vx = 0.0f, float vy = 0.0f) : velocity(vx, vy) {}
};