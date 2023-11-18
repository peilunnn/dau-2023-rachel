#ifndef HANDLE_SHOOTING_H
#define HANDLE_SHOOTING_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Utilities/include/app.h"

class HandleShooting {
public:
    static void Shoot(EntityManager& entityManager, Entity playerEntityId, CSimpleSprite* bulletSprite, float mouseX, float mouseY);

private:
    static int bulletCount;
    static constexpr int MAX_BULLETS = 10;
};

#endif // HANDLE_SHOOTING_H
