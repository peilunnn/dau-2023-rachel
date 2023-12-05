#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/SimpleSprite.h"
#include <memory>

class ShootingHandler : public System {
public:
    ShootingHandler() {
        m_subscribedEvents.insert("PlayerHitReloadingCircle");
    }

    static constexpr int MAX_BULLETS = 10;

    static void Shoot(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY);
    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;

    static float GetTimeSinceLastShot() { return s_timeSinceLastShot; }
    static void SetTimeSinceLastShot(float newTime) { s_timeSinceLastShot = newTime; }
    set<string> GetSubscribedEvents() const override {
        return m_subscribedEvents;
    }

private:
    set<string> m_subscribedEvents;
    static int s_bulletsShotSoFar;
    static constexpr float s_cooldownTimer = 0.5f;
    static float s_timeSinceLastShot;
    static constexpr float s_bulletSpeed = 1500.0f;

    void HandlePlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime);
};