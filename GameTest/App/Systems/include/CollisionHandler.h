#pragma once
#include "Components/include/Collider.h"
#include "Components/include/Transform.h"
#include "Managers/include/SystemManager.h"
using namespace std;

class CollisionHandler : public System {
public:
    static CollisionHandler& GetInstance() {
        static CollisionHandler instance;
        return instance;
    }
    CollisionHandler(CollisionHandler const&) = delete;
    void operator=(CollisionHandler const&) = delete;

    void Update(float deltaTime);

private:
    CollisionHandler() = default;

    bool IsColliding(Transform* transform1, Collider* collider1, Transform* transform2, Collider* collider2);
    void HandleCollisionEvent(EntityManager& entityManager, SystemManager& systemManager, EntityId firstEntityId, EntityId secondEntityId);
};
