#ifndef HANDLE_PLAYER_MOVEMENT_H
#define HANDLE_PLAYER_MOVEMENT_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"

class HandlePlayerMovement {
public:
    void Update(EntityManager& entityManager, float deltaTime);
};

#endif // HANDLE_PLAYER_MOVEMENT_H
