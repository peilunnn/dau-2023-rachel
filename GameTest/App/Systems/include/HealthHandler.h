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
    set<EventType> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

    int GetHealthReduction() const {
        return m_healthReduction;
    }
    void ResetPlayerHealth();

private:
    HealthHandler() {
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
    }

    set<EventType> m_subscribedEvents = {};
    const int m_healthReduction = 20;

    void HandleEnemyHitPlayer(EntityManager& entityManager);
};
