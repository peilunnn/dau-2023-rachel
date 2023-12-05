#pragma once
#include "Systems/include/System.h"
#include <set>

class EntityHandler : public System {
public:
    static EntityHandler& GetInstance() {
        static EntityHandler instance;
        return instance;
    }
    EntityHandler(EntityHandler const&) = delete;
    void operator=(EntityHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    EntityHandler() {
        m_subscribedEvents.insert("BulletHitEnemy");
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    set<string> m_subscribedEvents;
    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
    void HandleEnemyHitPlayer(Event event, EntityManager& entityManager, float deltaTime);
};
