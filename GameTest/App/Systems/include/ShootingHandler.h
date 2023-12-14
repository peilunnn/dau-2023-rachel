#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class ShootingHandler : public System
{
public:
    static ShootingHandler &GetInstance();
    ShootingHandler(ShootingHandler const &) = delete;
    void operator=(ShootingHandler const &) = delete;

    const int MAX_BULLETS = 10;

    void HandlePlayerShoot();
    void HandlePlayerHitReloadingCircle();
    void ResetBullets();

private:
    ShootingHandler() = default;

    int m_bulletsShotSoFar = 0;
    const float m_bulletSpeed = 2000.0f;
};
