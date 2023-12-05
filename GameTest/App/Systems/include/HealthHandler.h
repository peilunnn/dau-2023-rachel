#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class HealthHandler : public System {
public:
    HealthHandler() {
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    set<string> m_subscribedEvents;
    const int healthReduction = 20;

    void HandleEnemyHitPlayer(EntityManager& entityManager);
};