#pragma once
#include "Components/include/Screen.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using glm::vec2;

class MovementHandler : public System {
public:
    static MovementHandler& GetInstance();
    MovementHandler(MovementHandler const&) = delete;
    void operator=(MovementHandler const&) = delete;

    void Update(float deltaTime);
    void HandleEvent(const Event& event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    MovementHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
    }

    set<string> m_subscribedEvents = {};

    void HandlePlayerMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void HandleEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void HandleBulletMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void HandleBulletHitEnemy(EntityManager& entityManager, EntityId enemyEntityId, float deltaTime);
};
