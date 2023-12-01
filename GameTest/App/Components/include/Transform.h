#pragma once
#include "Component.h"
#include <glm/vec3.hpp>

class Transform : public Component {
public:
    Transform(const glm::vec3& newPos = glm::vec3(0.0f), const glm::vec3& newRot = glm::vec3(0.0f), const glm::vec3& newScl = glm::vec3(1.0f))
        : m_position(newPos), m_rotation(newRot), m_scale(newScl) {}

    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetRotation() const { return m_rotation; }
    glm::vec3 GetScale() const { return m_scale; }

    void SetPosition(const glm::vec3& newPos) { m_position = newPos; }
    void SetRotation(const glm::vec3& newRot) { m_rotation = newRot; }
    void SetScale(const glm::vec3& newScale) { m_scale = newScale; }

private:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};
