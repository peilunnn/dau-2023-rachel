#pragma once
#include "Components/include/Component.h"
#include <glm/glm.hpp>
using glm::vec2;

class Velocity : public Component {
public:
    Velocity(const vec2& newVelocity = vec2(0.0f))
        : m_velocity(newVelocity) {}

    vec2 GetVelocity() const { return m_velocity; }
    void SetVelocity(const vec2& newVelocity) { m_velocity = newVelocity; }

private:
    vec2 m_velocity = vec2(0.0f);
};