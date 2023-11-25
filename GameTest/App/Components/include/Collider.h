#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

enum class CollisionShape {
    SPHERE,
    CAPSULE,
};

enum class CollisionType {
    NONE = 0,
    PLAYER = 1 << 0,  // 0001
    ENEMY = 1 << 1,   // 0010
    BULLET = 1 << 2,   // 0100
    RELOADING_CIRCLE = 1 << 3 // 1000
};

class Collider : public Component {
public:
    CollisionShape collisionShape;
    CollisionType collisionType;
    int collisionMask;
    float radius;

    Collider() : collisionShape(CollisionShape::SPHERE), collisionType(CollisionType::NONE), collisionMask(0), radius(0.0f) {}
};


#endif // COLLIDER_H
