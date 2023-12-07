#pragma once
#include "Components/include/Component.h"
#include <glm/glm.hpp>
using glm::vec3;

class Transform : public Component {
public:
    Transform(const vec3& newPos = vec3(0.0f), const vec3& newRot = vec3(0.0f), const vec3& newScl = vec3(1.0f))
        : m_position(newPos), m_rotation(newRot), m_scale(newScl) {}

    vec3 GetPosition() const { return m_position; }
    vec3 GetRotation() const { return m_rotation; }
    vec3 GetScale() const { return m_scale; }

    void SetPosition(const vec3& newPos) { m_position = newPos; }
    void SetRotation(const vec3& newRot) { m_rotation = newRot; }
    void SetScale(const vec3& newScale) { m_scale = newScale; }

private:
    vec3 m_position;
    vec3 m_rotation;
    vec3 m_scale;
};
