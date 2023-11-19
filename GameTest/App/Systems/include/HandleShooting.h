#ifndef HANDLE_SHOOTING_H
#define HANDLE_SHOOTING_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Utilities/include/app.h"
#include "../../Systems/include/System.h"

class HandleShooting : public System {
public:
    static void Shoot(EntityManager& entityManager, Entity playerEntityId, CSimpleSprite* bulletSprite, float mouseX, float mouseY);

private:
    static int bulletCount;
    static constexpr int MAX_BULLETS = 10;
};

#endif // HANDLE_SHOOTING_H
