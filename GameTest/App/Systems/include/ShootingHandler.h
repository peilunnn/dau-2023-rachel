#ifndef HANDLE_SHOOTING_H
#define HANDLE_SHOOTING_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Utilities/include/app.h"
#include "../../Systems/include/System.h"

class ShootingHandler : public System {
private:
    static int bulletCount;
    static constexpr int MAX_BULLETS = 10;
    static float cooldownTimer;
    static float timeSinceLastShot;

public:
    static void Shoot(EntityManager& entityManager, Entity playerEntity, std::shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY);
    
    Type GetSystemType() const override {
        return System::Type::ShootingHandler;
    }
    static float GetTimeSinceLastShot() {
        return timeSinceLastShot;
    }
    static void SetTimeSinceLastShot(float time) {
        timeSinceLastShot = time;
    }
    static float GetMaxBullets() {
        return MAX_BULLETS;
    }
};

#endif // HANDLE_SHOOTING_H
