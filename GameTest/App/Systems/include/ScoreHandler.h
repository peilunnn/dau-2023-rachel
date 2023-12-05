#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class ScoreHandler : public System {
public:
    ScoreHandler() {
        subscribedEvents.insert("BulletHitEnemy");
    }

    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
    set<string> GetSubscribedEvents() const override {
        return subscribedEvents;
    }
private:
    set<string> subscribedEvents;
};