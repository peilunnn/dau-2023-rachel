#ifndef HANDLE_COLLISION_H
#define HANDLE_COLLISION_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Collider.h"
#include "../../Utilities/include/Helper.h"
#include "../../Systems/include/System.h"

class HandleCollision : public System 
{
public:
    void Update(EntityManager& entityManager, float deltaTime);
    bool IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1, std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2);
    void HandleCollisionEvent(EntityManager&, Entity entity1, Entity entity2);
};

#endif // HANDLE_COLLISION_H
