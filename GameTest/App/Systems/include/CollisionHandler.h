#pragma once
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
    void Update(EntityManager &entityManager, SystemManager &systemManager, float deltaTime);
    bool IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1, std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2);
    void HandleCollisionEvent(EntityManager &entityManager, SystemManager &systemManager, EntityId entity1Id, EntityId entity2Id);
    SystemType GetSystemType() const override
    {
        return SystemType::CollisionHandler;
    }
};