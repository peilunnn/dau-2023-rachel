#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/SimpleSprite.h"
#include <memory>

class ShootingHandler : public System {
public:
    static constexpr int MAX_BULLETS = 10;

    static void Shoot(EntityManager& entityManager, EntityId playerEntityId, std::shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY);
    void ProcessPlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime);

    static float GetTimeSinceLastShot() { return s_timeSinceLastShot; }
    static void SetTimeSinceLastShot(float newTime) { s_timeSinceLastShot = newTime; }

    SystemType GetSystemType() const override {
        return SystemType::ShootingHandler;
    }

private:
    static int s_bulletsShotSoFar;
    static constexpr float s_cooldownTimer = 0.5f;
    static float s_timeSinceLastShot;
};