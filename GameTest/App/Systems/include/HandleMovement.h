#ifndef HANDLE_MOVEMENT_H
#define HANDLE_MOVEMENT_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/Debug.h"

class HandleMovement {
public:
    void Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight);
    void HandlePlayerMovement(EntityManager& entityManager, float deltaTime);
    void HandleEnemyMovement(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight);
};

#endif // HANDLE_MOVEMENT_H