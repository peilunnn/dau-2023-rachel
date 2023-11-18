#ifndef HANDLE_ENEMY_MOVEMENT_H
#define HANDLE_ENEMY_MOVEMENT_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"

class HandleEnemyMovement {
public:
    void Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight);
};

#endif // HANDLE_ENEMY_MOVEMENT_H