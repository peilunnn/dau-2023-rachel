#pragma once
#include "Systems/include/System.h"
#include <set>

class EntityHandler : public System
{
public:
    EntityHandler() {
        subscribedEvents.insert("BulletHitEnemy");
        subscribedEvents.insert("EnemyHitPlayer");
    }

    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
    void HandleEnemyHitPlayer(Event event, EntityManager& entityManager, float deltaTime);
    set<string> GetSubscribedEvents() const override {
        return subscribedEvents;
    }

private:
    set<string> subscribedEvents;
};