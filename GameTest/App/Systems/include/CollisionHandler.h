#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <set>
#include "../../Managers/include/EntityManager.h"
#include "../../Managers/include/SystemManager.h"
#include "../../Components/include/Collider.h"
#include "../../Utilities/include/Helper.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/Event.h"

class CollisionHandler : public System 
{
public:
    void Update(EntityManager& entityManager, SystemManager& systemManager, float deltaTime);
    bool IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1, std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2);
    void HandleCollisionEvent(EntityManager& entityManager, SystemManager& systemManager, Entity entity1, Entity entity2);
    Type GetSystemType() const override {
        return System::Type::CollisionHandler;
    }
};

#endif // COLLISION_HANDLER_H
