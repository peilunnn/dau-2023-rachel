#pragma once
#include "Component.h"
#include "../App/Utilities/include/Enums.h"

class Collider : public Component {
public:
    CollisionShape collisionShape;
    CollisionType collisionType;
    int collisionMask;
    float radius;

    Collider() : collisionShape(CollisionShape::SPHERE), collisionType(CollisionType::NONE), collisionMask(0), radius(0.0f) {}
};