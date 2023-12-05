#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using glm::vec2;

class MovementHandler : public System {
public:
    MovementHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
    }

    void Update(EntityManager& entityManager, float deltaTime);
    static vec2 GetVelocity(EntityId entityId);
    static void SetVelocity(EntityId entityId, const vec2& velocity);
    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    set<string> m_subscribedEvents;
    static unordered_map<EntityId, glm::vec2> entityVelocities;

    void HandlePlayerMovement(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void HandleEnemyMovement(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void HandleBulletMovement(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void HandleBulletHitEnemy(EntityManager& entityManager, EntityId firstEntityId, EntityId secondEntityId, float deltaTime);
};
