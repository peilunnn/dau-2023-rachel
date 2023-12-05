#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/ScreenHandler.h"
#include "Systems/include/System.h"
#include <unordered_map>
using glm::vec2;

class MovementHandler : public System {
public:
    static MovementHandler& GetInstance() {
        static MovementHandler instance;
        return instance;
    }
    MovementHandler(MovementHandler const&) = delete;
    void operator=(MovementHandler const&) = delete;

    void Update(float deltaTime);
    vec2 GetVelocity(EntityId entityId);
    void SetVelocity(EntityId entityId, const vec2& velocity);
    void HandleEvent(const Event& event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    MovementHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
    }

    set<string> m_subscribedEvents;
    unordered_map<EntityId, glm::vec2> m_entityVelocities;

    void HandlePlayerMovement(EntityManager& entityManager, ScreenHandler& screenHandler, EntityId entityId, float deltaTime);
    void HandleEnemyMovement(EntityManager& entityManager, ScreenHandler& screenHandler, EntityId entityId, float deltaTime);
    void HandleBulletMovement(EntityManager& entityManager, ScreenHandler& screenHandler, EntityId entityId, float deltaTime);
    void HandleBulletHitEnemy(EntityManager& entityManager, EntityId firstEntityId, EntityId secondEntityId, float deltaTime);
};
