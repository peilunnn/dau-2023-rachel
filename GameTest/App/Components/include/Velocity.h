#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

class Velocity : public Component {
public:
    Velocity(const glm::vec2& newVelocity = glm::vec2(0.0f))
        : m_velocity(newVelocity) {}

    glm::vec2 GetVelocity() const { return m_velocity; }
    void SetVelocity(const glm::vec2& newVelocity) { m_velocity = newVelocity; }

private:
    glm::vec2 m_velocity;
};
