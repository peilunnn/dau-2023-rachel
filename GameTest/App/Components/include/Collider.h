#pragma once
#include "Component.h"
#include "../App/Utilities/include/Enums.h"

class Collider : public Component {
public:
    CollisionShape collisionShape = CollisionShape::SPHERE;
    CollisionType collisionType = CollisionType::NONE;
    int collisionMask = 0;
    float radius = 0.0f;

    Collider() = default;
};