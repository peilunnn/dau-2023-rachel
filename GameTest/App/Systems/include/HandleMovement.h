#ifndef HANDLE_MOVEMENT_H
#define HANDLE_MOVEMENT_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"

class HandleMovement {
public:
    void Update(EntityManager& entityManager, float deltaTime);
};

#endif // HANDLE_MOVEMENT_H
