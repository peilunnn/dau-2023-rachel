#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Collider : public Component {
public:
    Collider() = default;

    float GetRadius() const { return m_radius; }
    bool HasBeenCheckedForCollisions() const { return m_hasBeenCheckedForCollisions; }
    CollisionShape GetCollisionShape() const { return m_collisionShape; }

    void SetRadius(float newRadius) { m_radius = newRadius; }
    void SetCheckedForCollisions(bool checked) { m_hasBeenCheckedForCollisions = checked; }
    void SetCollisionShape(CollisionShape newShape) { m_collisionShape = newShape; }

private:
    float m_radius = 0.0f;
    bool m_hasBeenCheckedForCollisions = false;
    CollisionShape m_collisionShape = CollisionShape::Sphere;
};
