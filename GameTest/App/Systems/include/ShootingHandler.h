#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Systems/include/System.h"
using namespace std;

class ShootingHandler : public System
{
private:
    static int bulletsShotsSoFar;
    static constexpr int MAX_BULLETS = 10;
    static float cooldownTimer;
    static float timeSinceLastShot;

public:
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
    static int GetMaxBullets()
    {
        return MAX_BULLETS;
    }
    SystemType GetSystemType() const override
    {
        return SystemType::ShootingHandler;
    }
};