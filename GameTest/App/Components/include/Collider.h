#ifndef COLLIDER_H
#define COLLIDER_H

enum class CollisionShape {
    SPHERE,
};

class Collider {
public:
    CollisionShape collisionShape;
    int collisionMask;

    Collider() : collisionShape(CollisionShape::SPHERE), collisionMask(0) {}
};

#endif // COLLIDER_H
