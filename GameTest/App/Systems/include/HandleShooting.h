// HandleShootingSystem.h
#ifndef HANDLE_SHOOTING_SYSTEM_H
#define HANDLE_SHOOTING_SYSTEM_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Damage.h"

class HandleShootingSystem {
public:
    void Update(EntityManager& entityManager, float deltaTime) {
    }
};

#endif // HANDLE_SHOOTING_SYSTEM_H
