#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class HealthHandler : public System {
public:
    HealthHandler() {
        subscribedEvents.insert("EnemyHitPlayer");
    }

    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    void HandleEnemyHitPlayer(EntityManager& entityManager);
    set<string> GetSubscribedEvents() const override {
        return subscribedEvents;
    }

private:
    set<string> subscribedEvents;
    const int healthReduction = 20;
};