#pragma once
#include "../App/Utilities/include/Enums.h"
#include "Component.h"

class Collider : public Component {
public:
    Collider() = default;

    int GetCollisionMask() const { return m_collisionMask; }
    float GetRadius() const { return m_radius; }
    CollisionShape GetCollisionShape() const { return m_collisionShape; }
    CollisionType GetCollisionType() const { return m_collisionType; }

    void SetCollisionMask(int newMask) { m_collisionMask = newMask; }
    void SetRadius(float newRadius) { m_radius = newRadius; }
    void SetCollisionShape(CollisionShape newShape) { m_collisionShape = newShape; }
    void SetCollisionType(CollisionType newType) { m_collisionType = newType; }

private:
    int m_collisionMask = 0;
    float m_radius = 0.0f;
    CollisionShape m_collisionShape = CollisionShape::Sphere;
    CollisionType m_collisionType = CollisionType::None;
};
