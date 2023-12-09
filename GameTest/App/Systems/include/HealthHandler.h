#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include <set>

class HealthHandler : public System {
public:
    static HealthHandler& GetInstance();
    HealthHandler(HealthHandler const&) = delete;
    void operator=(HealthHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

    int GetHealthReduction() const {
        return m_healthReduction;
    }

private:
    HealthHandler() {
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    set<string> m_subscribedEvents = {};
    const int m_healthReduction = 20;

    void HandleEnemyHitPlayer(EntityManager& entityManager);
};
