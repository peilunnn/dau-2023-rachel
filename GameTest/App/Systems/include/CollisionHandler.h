#pragma once
#include "Components/include/Transform.h"
#include "Components/include/Collider.h"
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

    bool IsColliding(shared_ptr<Transform> transform1, shared_ptr<Collider> collider1, shared_ptr<Transform> transform2, shared_ptr<Collider> collider2);
    void HandleCollisionEvent(EntityManager& entityManager, SystemManager& systemManager, EntityId firstEntityId, EntityId secondEntityId);
};
