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
    
    void HandleEvent(const Event& event, float deltaTime) override;
    void HandlePlayerShoot();
    void ResetBullets();

private:
    ShootingHandler()
    {
        m_subscribedEvents.insert(EventType::PlayerHitAmmoPickup);
    }

    int m_bulletsShotSoFar = 0;
    const float m_bulletSpeed = 2000.0f;

    void HandlePlayerHitAmmoPickup();
};
