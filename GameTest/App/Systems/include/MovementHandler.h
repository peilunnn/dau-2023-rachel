#ifndef MOVEMENT_HANDLER_H
#define MOVEMENT_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/Helper.h"
#include "../../Systems/include/System.h"

class MovementHandler : public System {
public:
    void Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight);
    void HandlePlayerMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight);
    void HandleEnemyMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight);
    void HandleBulletMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight);
    Type GetSystemType() const override {
        return System::Type::MovementHandler;
    }
};

#endif // MOVEMENT_HANDLER_H