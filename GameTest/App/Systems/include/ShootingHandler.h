#ifndef SHOOTING_HANDLER_H
#define SHOOTING_HANDLER_H

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
    static int GetMaxBullets() {
        return MAX_BULLETS;
    }
};

#endif // SHOOTING_HANDLER_H
