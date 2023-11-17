#ifndef COLLIDER_H
#define COLLIDER_H

#include "Component.h"

enum class CollisionShape {
    SPHERE,
};

class Collider : public Component {
public:
    CollisionShape collisionShape;
    int collisionMask;

    Collider() : collisionShape(CollisionShape::SPHERE), collisionMask(0) {}
};

#endif // COLLIDER_H
