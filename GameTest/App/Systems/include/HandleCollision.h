#ifndef HANDLE_COLLISION_H
#define HANDLE_COLLISION_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Collider.h"

class HandleCollision {
public:
    void Update(EntityManager& entityManager, float deltaTime);
    bool IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1,
        std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2);
    void HandleCollisionEvent(Entity entity1, Entity entity2);
};

#endif // HANDLE_COLLISION_H
