#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include <set>

class ScoreHandler : public System {
public:
    static ScoreHandler& GetInstance();
    ScoreHandler(ScoreHandler const&) = delete;
    void operator=(ScoreHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

    void ResetScore();

private:
    ScoreHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
    }

    set<string> m_subscribedEvents = {};

    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
};
