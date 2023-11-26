#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/Helper.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/ScreenHandler.h"

class MovementHandler : public System
{
public:
    void Update(EntityManager &entityManager, float deltaTime);
    void HandlePlayerMovement(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void HandleEnemyMovement(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void HandleBulletMovement(EntityManager &entityManager, EntityId entityId, float deltaTime);
    SystemType GetSystemType() const override
    {
        return SystemType::MovementHandler;
    }
};