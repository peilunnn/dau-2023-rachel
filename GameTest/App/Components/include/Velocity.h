#pragma once
#include <glm/vec2.hpp>
#include "Component.h"

class Velocity : public Component {
public:
    glm::vec2 velocity = glm::vec2(0.0f);

    Velocity(const glm::vec2& vel = glm::vec2(0.0f))
        : velocity(vel) {}
};