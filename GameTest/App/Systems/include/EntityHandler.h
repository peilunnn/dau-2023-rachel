#pragma once
#include "Systems/include/System.h"
#include <set>

class EntityHandler : public System
{
public:
    EntityHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    set<string> m_subscribedEvents;
    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
    void HandleEnemyHitPlayer(Event event, EntityManager& entityManager, float deltaTime);
};