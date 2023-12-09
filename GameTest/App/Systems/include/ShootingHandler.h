#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class ShootingHandler : public System {
public:
    static ShootingHandler& GetInstance();
    ShootingHandler(ShootingHandler const&) = delete;
    void operator=(ShootingHandler const&) = delete;

    const int MAX_BULLETS = 10;

    void HandleEvent(const Event& event, float deltaTime) override;

    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    ShootingHandler() {
        m_subscribedEvents.insert("PlayerShoot");
        m_subscribedEvents.insert("PlayerHitReloadingCircle");
    }

    set<string> m_subscribedEvents = {};
    int m_bulletsShotSoFar = 0;
    const float m_bulletSpeed = 2000.0f;

    void HandlePlayerShoot(EntityManager& entityManager);
    void HandlePlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime);
};
