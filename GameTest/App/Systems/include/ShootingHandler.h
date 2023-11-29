#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using namespace std;

class ShootingHandler : public System
{
private:
    static int bulletsShotSoFar;
    static constexpr float cooldownTimer = 0.5f;
    static float timeSinceLastShot;

public:
    static constexpr int MAX_BULLETS = 10;

    static void Shoot(EntityManager &entityManager, EntityId playerEntity, shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY);
    void ProcessPlayerHitReloadingCircle(EntityManager &entityManager, float deltaTime);

    static float GetTimeSinceLastShot()
    {
        return timeSinceLastShot;
    }
    static void SetTimeSinceLastShot(float time)
    {
        timeSinceLastShot = time;
    }
    SystemType GetSystemType() const override
    {
        return SystemType::ShootingHandler;
    }
};