#ifndef HANDLE_MOVEMENT_H
#define HANDLE_MOVEMENT_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/Helper.h"

class HandleMovement {
public:
    void Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight);
    void HandlePlayerMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight);
    void HandleEnemyMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight);
    void HandleBulletMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight);
};

#endif // HANDLE_MOVEMENT_H