#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class ScoreHandler : public System {
public:
    ScoreHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
    }

    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }
private:
    set<string> m_subscribedEvents;

    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
};