#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/SimpleSprite.h"
#include <memory>

class ShootingHandler : public System {
public:
    static ShootingHandler& GetInstance() {
        static ShootingHandler instance;
        return instance;
    }
    ShootingHandler(ShootingHandler const&) = delete;
    void operator=(ShootingHandler const&) = delete;

    const int MAX_BULLETS = 10;

    void Shoot(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY);
    void HandleEvent(const Event& event, float deltaTime) override;

    float GetTimeSinceLastShot() const { return m_timeSinceLastShot; }
    void SetTimeSinceLastShot(float newTime) { m_timeSinceLastShot = newTime; }
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    ShootingHandler() {
        m_subscribedEvents.insert("PlayerHitReloadingCircle");
    }

    set<string> m_subscribedEvents;
    
    int m_bulletsShotSoFar = 0;
    const float m_cooldownTimer = 0.5f;
    float m_timeSinceLastShot = 0.0f;
    const float m_bulletSpeed = 1500.0f;

    void HandlePlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime);
};
